#include <unity.h>
#include "Config.h"
#include "MqttTopics.h"

void test_config_constants() {
    TEST_ASSERT_EQUAL_UINT8(13, cfg::PIN_PWM_LED);
    TEST_ASSERT_EQUAL_UINT8(15, cfg::PIN_SSR_FLOOR);
    TEST_ASSERT_EQUAL_UINT16(1023, cfg::PWM_RANGE);
    TEST_ASSERT_EQUAL_UINT32(30000, cfg::FLOOR_INTERVAL_MS);
    TEST_ASSERT_EQUAL_UINT32(10000, cfg::PIR_WARMUP_MS);
    TEST_ASSERT_EQUAL_STRING("ESP8266_Smart_Hall", cfg::MQTT_CLIENT_ID);
}

void test_mqtt_topics() {
    TEST_ASSERT_EQUAL_STRING("home/hall/light/light", mqtt::LIGHT_CMD);
    TEST_ASSERT_EQUAL_STRING("home/hall/floor/targetTemp", mqtt::FLOOR_TARGET);
    TEST_ASSERT_EQUAL_STRING("home/hall/state", mqtt::TELEMETRY_STATE);
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_config_constants);
    RUN_TEST(test_mqtt_topics);
    return UNITY_END();
}
