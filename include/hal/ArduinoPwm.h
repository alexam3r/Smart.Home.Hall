#pragma once

#include <cstdint>

#include "core/LightController.h"

class ArduinoPwm : public IPwmOutput {
public:
    void begin();
    void writeDuty(uint16_t duty) override;
};
