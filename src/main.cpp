#include <Arduino.h>

#include "Config.h"

void setup() {
    Serial.begin(115200);

    // ШИМ ленты: 4 кГц, 10 бит, прямая логика (0 — выключено).
    analogWriteFreq(cfg::PWM_FREQ_HZ);
    analogWriteRange(cfg::PWM_RANGE);
    pinMode(cfg::PIN_PWM_LED, OUTPUT);
    analogWrite(cfg::PIN_PWM_LED, 0);

    // Безопасный старт: реле пола выключено, PIR на вход.
    pinMode(cfg::PIN_SSR_FLOOR, OUTPUT);
    digitalWrite(cfg::PIN_SSR_FLOOR, LOW);
    pinMode(cfg::PIN_PIR, INPUT);

    Serial.println("[SYS] Argon skeleton ready");
}

void loop() {
}
