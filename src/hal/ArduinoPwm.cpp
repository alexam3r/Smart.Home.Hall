#include "hal/ArduinoPwm.h"

#include <Arduino.h>

#include "Config.h"

void ArduinoPwm::begin() {
    analogWriteFreq(cfg::PWM_FREQ_HZ);
    analogWriteRange(cfg::PWM_RANGE);
    pinMode(cfg::PIN_PWM_LED, OUTPUT);
    analogWrite(cfg::PIN_PWM_LED, 0);
}

void ArduinoPwm::writeDuty(uint16_t duty) {
    analogWrite(cfg::PIN_PWM_LED, constrain(duty, 0, cfg::PWM_RANGE));
}
