#pragma once

#include <Adafruit_AM2320.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "core/SensorManager.h"

class ArduinoSensors : public ISensorBus {
public:
    ArduinoSensors();

    void begin();

    void requestFloorConversion() override;
    float readFloorTempC() override;
    bool readAir(float& tempC, float& humPct) override;
    bool readPir() override;

private:
    OneWire oneWire_;
    DallasTemperature floorSensors_;
    Adafruit_AM2320 am2320_;
};
