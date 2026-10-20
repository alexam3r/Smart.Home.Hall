#include "core/TelemetryFormatter.h"

#include <ArduinoJson.h>

#include <cstdio>

namespace {

float truncToTenths(float value) {
    return static_cast<float>(static_cast<int>(value * 10.0f)) / 10.0f;
}

}  // namespace

size_t formatTelemetry(const TelemetryData& data, char* out, size_t outSize) {
    StaticJsonDocument<512> doc;

    doc["light_state"] = data.lightOn ? "ON" : "OFF";
    doc["light_move_state"] = data.lightMotionMode ? "ON" : "OFF";
    doc["floor_state"] = data.floorMaster ? "ON" : "OFF";
    doc["floor_move_state"] = data.floorMotionMode ? "ON" : "OFF";
    doc["floor_heat_state"] = data.floorHeat ? "ON" : "OFF";

    char floorTempBuf[10] = {0};
    char airTempBuf[10] = {0};
    char airHumBuf[10] = {0};

    if (data.floorValid) {
        snprintf(floorTempBuf, sizeof(floorTempBuf), "%.1f", truncToTenths(data.floorTempC));
        doc["floor_temp"] = serialized(floorTempBuf);
    } else {
        doc["floor_temp"] = nullptr;
    }

    if (data.airValid) {
        snprintf(airTempBuf, sizeof(airTempBuf), "%.1f", truncToTenths(data.airTempC));
        snprintf(airHumBuf, sizeof(airHumBuf), "%.1f", truncToTenths(data.airHumPct));
        doc["air_temp"] = serialized(airTempBuf);
        doc["air_hum"] = serialized(airHumBuf);
    } else {
        doc["air_temp"] = nullptr;
        doc["air_hum"] = nullptr;
    }

    doc["pir_raw"] = data.pirRaw;
    doc["since_move_s"] = data.sinceMoveS;
    doc["uptime"] = data.uptimeS;
    doc["heap"] = data.heapBytes;

    return serializeJson(doc, out, outSize);
}
