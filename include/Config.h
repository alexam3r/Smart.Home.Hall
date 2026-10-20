#pragma once

#include <cstdint>

// Пины Wemos D1 Mini в номерах GPIO.
namespace cfg {

constexpr uint8_t PIN_SCL = 5;          // D1
constexpr uint8_t PIN_SDA = 4;          // D2
constexpr uint8_t PIN_DS18B20 = 14;     // D5
constexpr uint8_t PIN_PIR = 12;         // D6
constexpr uint8_t PIN_PWM_LED = 13;     // D7
constexpr uint8_t PIN_SSR_FLOOR = 15;   // D8

constexpr uint32_t PWM_FREQ_HZ = 4000;
constexpr uint16_t PWM_RANGE = 1023;

constexpr uint32_t LIGHT_FADE_MS = 2000;
constexpr uint32_t LIGHT_STEP_MS = 20;
constexpr uint32_t LIGHT_PIR_TIMEOUT_MS = 60000;
constexpr uint32_t LIGHT_SAFETY_TIMEOUT_MS = 3600000;
constexpr uint8_t LIGHT_DEFAULT_BRIGHTNESS = 100;

constexpr uint32_t FLOOR_MOTION_TIMEOUT_MS = 3600000;
constexpr uint32_t FLOOR_SAFETY_TIMEOUT_MS = 43200000;
constexpr uint32_t FLOOR_STALE_TIMEOUT_MS = 90000;
constexpr float FLOOR_HYSTERESIS_C = 0.5f;
constexpr float FLOOR_DEFAULT_TARGET_C = 30.0f;

constexpr uint32_t PIR_WARMUP_MS = 10000;
constexpr uint32_t FLOOR_INTERVAL_MS = 30000;
constexpr uint32_t FLOOR_CONVERSION_MS = 850;  // 750 по даташиту + запас
constexpr uint32_t AIR_INTERVAL_MS = 15000;
constexpr uint32_t TELEMETRY_INTERVAL_MS = 10000;

constexpr uint32_t MQTT_RETRY_MS = 5000;
constexpr uint32_t MQTT_LOST_REBOOT_MS = 300000;
constexpr char MQTT_CLIENT_ID[] = "ESP8266_Smart_Hall";
constexpr uint16_t TELEMETRY_BUFFER_SIZE = 512;

}  // namespace cfg
