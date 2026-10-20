#include <Arduino.h>

#include <cstdio>

#include "Config.h"
#include "MqttTopics.h"
#include "NetworkManager.h"
#include "Secrets.h"
#include "core/FloorThermostat.h"
#include "core/LightController.h"
#include "core/TelemetryFormatter.h"
#include "hal/ArduinoPwm.h"
#include "hal/ArduinoRelay.h"
#include "hal/ArduinoSensors.h"

namespace {

constexpr SensorConfig kSensorConfig{
    cfg::FLOOR_INTERVAL_MS,
    cfg::FLOOR_CONVERSION_MS,
    cfg::AIR_INTERVAL_MS,
    cfg::PIR_WARMUP_MS,
};

constexpr LightConfig kLightConfig{
    cfg::LIGHT_FADE_MS,
    cfg::LIGHT_STEP_MS,
    cfg::LIGHT_PIR_TIMEOUT_MS,
    cfg::LIGHT_SAFETY_TIMEOUT_MS,
    cfg::LIGHT_DEFAULT_BRIGHTNESS,
    cfg::PWM_RANGE,
};

constexpr FloorConfig kFloorConfig{
    cfg::FLOOR_HYSTERESIS_C,
    cfg::FLOOR_MOTION_TIMEOUT_MS,
    cfg::FLOOR_SAFETY_TIMEOUT_MS,
    cfg::FLOOR_STALE_TIMEOUT_MS,
    cfg::FLOOR_DEFAULT_TARGET_C,
};

class SerialLightObserver : public ILightObserver {
public:
    void attach(NetworkManager* network) { network_ = network; }

    void onLightStateChanged(bool on) override {
        Serial.printf("[LIGHT] state -> %s\n", on ? "ON" : "OFF");
        if (network_ != nullptr) {
            network_->publish(mqtt::LIGHT_STATE, on ? "ON" : "OFF", true);
        }
    }

    void onBrightnessSettled(uint8_t percent) override {
        Serial.printf("[LIGHT] brightness settled -> %u%%\n", percent);
        if (network_ != nullptr) {
            char payload[4] = {0};
            snprintf(payload, sizeof(payload), "%u", static_cast<unsigned>(percent));
            network_->publish(mqtt::LIGHT_BRIGHTNESS_STATE, payload, true);
        }
    }

private:
    NetworkManager* network_ = nullptr;
};

class SerialFloorObserver : public IFloorObserver {
public:
    void attach(NetworkManager* network) { network_ = network; }

    void onMasterChanged(bool on) override {
        Serial.printf("[FLOOR] master -> %s\n", on ? "ON" : "OFF");
        if (network_ != nullptr) {
            network_->publish(mqtt::FLOOR_STATE, on ? "ON" : "OFF", true);
        }
    }

    void onHeatChanged(bool on) override {
        Serial.printf("[FLOOR] relay -> %s\n", on ? "ON" : "OFF");
        if (network_ != nullptr) {
            network_->publish(mqtt::FLOOR_HEAT_STATE, on ? "ON" : "OFF", true);
        }
    }

private:
    NetworkManager* network_ = nullptr;
};

class CommandSink : public IMqttCommandSink {
public:
    void attach(LightController* light, FloorThermostat* floor) {
        light_ = light;
        floor_ = floor;
    }

    void onLightPower(bool on) override {
        if (light_ != nullptr) light_->setPower(on, millis());
    }
    void onLightBrightness(uint8_t percent) override {
        if (light_ != nullptr) light_->setBrightness(percent, millis());
    }
    void onLightMotionMode(bool on) override {
        if (light_ != nullptr) light_->setMotionMode(on, millis());
    }
    void onFloorPower(bool on) override {
        if (floor_ != nullptr) floor_->setMaster(on, millis());
    }
    void onFloorMotionMode(bool on) override {
        if (floor_ != nullptr) floor_->setMotionMode(on, millis());
    }
    void onFloorTargetTemp(float targetC) override {
        if (floor_ != nullptr) floor_->setTarget(targetC, millis());
    }

private:
    LightController* light_ = nullptr;
    FloorThermostat* floor_ = nullptr;
};

ArduinoPwm pwm;
ArduinoRelay relay;
ArduinoSensors sensorBus;

SerialLightObserver lightObserver;
SerialFloorObserver floorObserver;

LightController light(pwm, lightObserver, kLightConfig, 0);
FloorThermostat floorThermostat(relay, floorObserver, kFloorConfig, 0);

CommandSink commandSink;
NetworkManager network(commandSink, WIFI_SSID, WIFI_PASS, MQTT_USER, MQTT_PASS,
                       cfg::MQTT_CLIENT_ID);

SensorManager sensors(sensorBus, 0, kSensorConfig);

uint32_t lastSensorLogMs = 0;
uint32_t lastFloorSampleMs = 0;
uint32_t lastTelemetryMs = 0;

void publishTelemetry(uint32_t now, const SensorSnapshot& snapshot) {
    TelemetryData data{};
    data.lightOn = light.isOn();
    data.lightMotionMode = light.motionMode();
    data.floorMaster = floorThermostat.master();
    data.floorMotionMode = floorThermostat.motionMode();
    data.floorHeat = floorThermostat.heat();

    data.floorValid = snapshot.floorValid;
    data.floorTempC = snapshot.floorTempC;
    data.airValid = snapshot.airValid;
    data.airTempC = snapshot.airTempC;
    data.airHumPct = snapshot.airHumPct;

    data.pirRaw = snapshot.pirRaw;
    data.sinceMoveS = light.secondsSinceActivity(now);
    data.uptimeS = now / 1000;
    data.heapBytes = ESP.getFreeHeap();

    char buffer[cfg::TELEMETRY_BUFFER_SIZE];
    formatTelemetry(data, buffer, sizeof(buffer));
    network.publish(mqtt::TELEMETRY_STATE, buffer, false);
}

}  // namespace

void setup() {
    Serial.begin(115200);

    pwm.begin();
    relay.begin();
    pinMode(cfg::PIN_PIR, INPUT);
    sensorBus.begin();

    commandSink.attach(&light, &floorThermostat);
    lightObserver.attach(&network);
    floorObserver.attach(&network);
    network.begin();

    Serial.println("[SYS] Argon ready");
}

void loop() {
    const uint32_t now = millis();

    network.update(now);
    sensors.update(now);
    const SensorSnapshot& snapshot = sensors.snapshot();

    if (snapshot.floorValid && snapshot.floorSampleMs != lastFloorSampleMs) {
        lastFloorSampleMs = snapshot.floorSampleMs;
        floorThermostat.onSample(snapshot.floorTempC, snapshot.floorSampleMs);
    }

    if (sensors.takeMotionStarted()) {
        Serial.println("[PIR] motion started");
        network.publish(mqtt::LIGHT_MOVE, "ON", false);
        light.onMotion(now);
        floorThermostat.onMotion(now);
    }
    if (sensors.takeMotionStopped()) {
        Serial.println("[PIR] motion stopped");
        network.publish(mqtt::LIGHT_MOVE, "OFF", false);
    }

    light.update(now);
    floorThermostat.update(now);

    if (now - lastTelemetryMs >= cfg::TELEMETRY_INTERVAL_MS) {
        lastTelemetryMs = now;
        publishTelemetry(now, snapshot);
    }

    if (now - lastSensorLogMs >= 5000) {
        lastSensorLogMs = now;
        Serial.printf("[SENS] floor=%.1fC valid=%d air=%.1fC/%.1f%% valid=%d pir=%d\n",
                      snapshot.floorTempC, static_cast<int>(snapshot.floorValid),
                      snapshot.airTempC, snapshot.airHumPct, static_cast<int>(snapshot.airValid),
                      static_cast<int>(snapshot.pirRaw));
    }
}
