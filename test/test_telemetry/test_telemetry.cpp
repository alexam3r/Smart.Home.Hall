#include <unity.h>

#include <cstddef>

#include "core/TelemetryFormatter.h"

void test_full_json_has_fixed_key_order() {
    TelemetryData data{};
    data.lightOn = true;
    data.lightMotionMode = true;
    data.floorMaster = true;
    data.floorMotionMode = true;
    data.floorHeat = false;
    data.floorValid = true;
    data.floorTempC = 27.44f;
    data.airValid = true;
    data.airTempC = 22.14f;
    data.airHumPct = 41.54f;
    data.pirRaw = false;
    data.sinceMoveS = 37;
    data.uptimeS = 86400;
    data.heapBytes = 31240;

    char buffer[512];
    const size_t length = formatTelemetry(data, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL_STRING(
        "{\"light_state\":\"ON\",\"light_move_state\":\"ON\",\"floor_state\":\"ON\","
        "\"floor_move_state\":\"ON\",\"floor_heat_state\":\"OFF\",\"floor_temp\":27.4,"
        "\"air_temp\":22.1,\"air_hum\":41.5,\"pir_raw\":false,\"since_move_s\":37,"
        "\"uptime\":86400,\"heap\":31240}",
        buffer);
    TEST_ASSERT_TRUE(length > 0);
    TEST_ASSERT_TRUE(length < sizeof(buffer));
}

void test_invalid_sensors_are_null() {
    TelemetryData data{};
    data.floorValid = false;
    data.airValid = false;

    char buffer[512];
    formatTelemetry(data, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL_STRING(
        "{\"light_state\":\"OFF\",\"light_move_state\":\"OFF\",\"floor_state\":\"OFF\","
        "\"floor_move_state\":\"OFF\",\"floor_heat_state\":\"OFF\",\"floor_temp\":null,"
        "\"air_temp\":null,\"air_hum\":null,\"pir_raw\":false,\"since_move_s\":0,"
        "\"uptime\":0,\"heap\":0}",
        buffer);
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_full_json_has_fixed_key_order);
    RUN_TEST(test_invalid_sensors_are_null);
    return UNITY_END();
}
