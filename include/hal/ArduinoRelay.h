#pragma once

#include "core/FloorThermostat.h"

class ArduinoRelay : public IRelayOutput {
public:
    void begin();
    void writeRelay(bool on) override;
};
