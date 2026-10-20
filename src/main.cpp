#include <Arduino.h>

#include "Config.h"
#include "core/FloorThermostat.h"
#include "core/LightController.h"
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
    void onLightStateChanged(bool on) override {
        Serial.printf("[LIGHT] state -> %s\n", on ? "ON" : "OFF");
    }
    void onBrightnessSettled(uint8_t percent) override {
        Serial.printf("[LIGHT] brightness settled -> %u%%\n", percent);
    }
};

class SerialFloorObserver : public IFloorObserver {
public:
    void onMasterChanged(bool on) override {
        Serial.printf("[FLOOR] master -> %s\n", on ? "ON" : "OFF");
    }
    void onHeatChanged(bool on) override {
        Serial.printf("[FLOOR] relay -> %s\n", on ? "ON" : "OFF");
    }
};

ArduinoPwm pwm;
SerialLightObserver lightObserver;
LightController light(pwm, lightObserver, kLightConfig, 0);

ArduinoRelay relay;
SerialFloorObserver floorObserver;
FloorThermostat floorThermostat(relay, floorObserver, kFloorConfig, 0);

ArduinoSensors sensorBus;
SensorManager sensors(sensorBus, 0, kSensorConfig);

uint32_t lastSensorLogMs = 0;
uint32_t lastFloorSampleMs = 0;

}  // namespace

void setup() {
    Serial.begin(115200);

    pwm.begin();
    relay.begin();

    pinMode(cfg::PIN_PIR, INPUT);

    sensorBus.begin();
    Serial.println("[SYS] Argon sensors + light + floor ready");
}

void loop() {
    const uint32_t now = millis();
    sensors.update(now);
    const SensorSnapshot& snapshot = sensors.snapshot();

    if (snapshot.floorValid && snapshot.floorSampleMs != lastFloorSampleMs) {
        lastFloorSampleMs = snapshot.floorSampleMs;
        floorThermostat.onSample(snapshot.floorTempC, snapshot.floorSampleMs);
    }

    if (sensors.takeMotionStarted()) {
        Serial.println("[PIR] motion started");
        light.onMotion(now);
        floorThermostat.onMotion(now);
    }
    if (sensors.takeMotionStopped()) {
        Serial.println("[PIR] motion stopped");
    }

    light.update(now);
    floorThermostat.update(now);

    if (now - lastSensorLogMs >= 5000) {
        lastSensorLogMs = now;
        Serial.printf("[SENS] floor=%.1fC valid=%d air=%.1fC/%.1f%% valid=%d pir=%d\n",
                      snapshot.floorTempC, static_cast<int>(snapshot.floorValid),
                      snapshot.airTempC, snapshot.airHumPct, static_cast<int>(snapshot.airValid),
                      static_cast<int>(snapshot.pirRaw));
    }
}
