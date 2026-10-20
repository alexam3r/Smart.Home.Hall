#pragma once

#include <cstddef>
#include <cstdint>

struct TelemetryData {
    bool lightOn;
    bool lightMotionMode;
    bool floorMaster;
    bool floorMotionMode;
    bool floorHeat;

    bool floorValid;
    float floorTempC;
    bool airValid;
    float airTempC;
    float airHumPct;

    bool pirRaw;
    uint32_t sinceMoveS;
    uint32_t uptimeS;
    uint32_t heapBytes;
};

// Формирует JSON в фиксированном порядке ключей. Возвращает длину строки.
size_t formatTelemetry(const TelemetryData& data, char* out, size_t outSize);
