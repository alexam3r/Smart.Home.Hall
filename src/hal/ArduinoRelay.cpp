#include "hal/ArduinoRelay.h"

#include <Arduino.h>

#include "Config.h"

void ArduinoRelay::begin() {
    pinMode(cfg::PIN_SSR_FLOOR, OUTPUT);
    digitalWrite(cfg::PIN_SSR_FLOOR, LOW);
}

void ArduinoRelay::writeRelay(bool on) {
    digitalWrite(cfg::PIN_SSR_FLOOR, on ? HIGH : LOW);
}
