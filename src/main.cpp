#include <Arduino.h>

#include "Config.h"
#include "core/LightController.h"
#include "hal/ArduinoPwm.h"
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

class SerialLightObserver : public ILightObserver {
public:
    void onLightStateChanged(bool on) override {
        Serial.printf("[LIGHT] state -> %s\n", on ? "ON" : "OFF");
    }
    void onBrightnessSettled(uint8_t percent) override {
        Serial.printf("[LIGHT] brightness settled -> %u%%\n", percent);
    }
};

ArduinoPwm pwm;
SerialLightObserver lightObserver;
LightController light(pwm, lightObserver, kLightConfig, 0);

ArduinoSensors sensorBus;
SensorManager sensors(sensorBus, 0, kSensorConfig);
uint32_t lastSensorLogMs = 0;

}  // namespace

void setup() {
    Serial.begin(115200);

    pwm.begin();

    // Безопасный старт: реле пола выключено, PIR на вход.
    pinMode(cfg::PIN_SSR_FLOOR, OUTPUT);
    digitalWrite(cfg::PIN_SSR_FLOOR, LOW);
    pinMode(cfg::PIN_PIR, INPUT);

    sensorBus.begin();
    Serial.println("[SYS] Argon sensors + light ready");
}

void loop() {
    const uint32_t now = millis();
    sensors.update(now);
    light.update(now);

    if (sensors.takeMotionStarted()) {
        Serial.println("[PIR] motion started");
    }
    if (sensors.takeMotionStopped()) {
        Serial.println("[PIR] motion stopped");
    }

    if (now - lastSensorLogMs >= 5000) {
        lastSensorLogMs = now;
        const SensorSnapshot& s = sensors.snapshot();
        Serial.printf("[SENS] floor=%.1fC valid=%d air=%.1fC/%.1f%% valid=%d pir=%d\n",
                      s.floorTempC, static_cast<int>(s.floorValid),
                      s.airTempC, s.airHumPct, static_cast<int>(s.airValid),
                      static_cast<int>(s.pirRaw));
    }
}
