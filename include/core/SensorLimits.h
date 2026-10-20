#pragma once

#include <cmath>

namespace argon {

constexpr float kMinValidFloorTempC = 5.0f;
constexpr float kMaxValidFloorTempC = 100.0f;

// DS18B20: -127 — нет устройства, 85 — незавершённое преобразование.
// Ниже 5 °C в жилом помещении считаем ошибкой датчика (решение владельца).
inline bool isValidFloorTemp(float tempC) {
    return !std::isnan(tempC) && tempC >= kMinValidFloorTempC &&
           tempC <= kMaxValidFloorTempC && tempC != 85.0f;
}

}  // namespace argon
