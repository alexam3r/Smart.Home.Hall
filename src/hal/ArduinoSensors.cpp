#include "hal/ArduinoSensors.h"

#include <Arduino.h>
#include <Wire.h>

#include "Config.h"

ArduinoSensors::ArduinoSensors()
    : oneWire_(cfg::PIN_DS18B20), floorSensors_(&oneWire_), am2320_() {}

void ArduinoSensors::begin() {
    Wire.begin(cfg::PIN_SDA, cfg::PIN_SCL);
    Wire.setClock(100000);

    am2320_.begin();
    floorSensors_.begin();
    // Преобразование запускается отдельно, ожидание — в SensorManager.
    floorSensors_.setWaitForConversion(false);
}

void ArduinoSensors::requestFloorConversion() {
    floorSensors_.requestTemperatures();
}

float ArduinoSensors::readFloorTempC() {
    return floorSensors_.getTempCByIndex(0);
}

bool ArduinoSensors::readAir(float& tempC, float& humPct) {
    // readRegister32 сам выполняет пробуждение AM2320; отдельный ACK-чек
    // до пробуждения ненадёжен (спящий датчик может не ответить).
    const uint32_t data = am2320_.readRegister32(0x00);
    if (data == 0xFFFFFFFFu) {
        return false;
    }

    const int16_t rawTemp = (data & 0x8000u)
                                ? static_cast<int16_t>(-static_cast<int16_t>(data & 0x7FFFu))
                                : static_cast<int16_t>(data & 0xFFFFu);
    tempC = static_cast<float>(rawTemp) / 10.0f;
    humPct = static_cast<float>(data >> 16) / 10.0f;
    return true;
}

bool ArduinoSensors::readPir() {
    return digitalRead(cfg::PIN_PIR) == HIGH;
}
