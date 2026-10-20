#include <unity.h>

#include <cmath>
#include <initializer_list>

#include "core/SensorManager.h"

namespace {

constexpr SensorConfig kCfg{30000, 750, 15000, 10000};

class FakeBus : public ISensorBus {
public:
    int floorRequests = 0;
    int airReads = 0;
    float floorTemp = 27.5f;
    bool airOk = true;
    float airTemp = 22.0f;
    float airHum = 40.0f;
    bool pir = false;

    void requestFloorConversion() override { ++floorRequests; }
    float readFloorTempC() override { return floorTemp; }
    bool readAir(float& tempC, float& humPct) override {
        ++airReads;
        if (!airOk) return false;
        tempC = airTemp;
        humPct = airHum;
        return true;
    }
    bool readPir() override { return pir; }
};

void test_first_update_requests_floor_and_reads_air() {
    FakeBus bus;
    SensorManager sm(bus, 0, kCfg);

    sm.update(0);

    TEST_ASSERT_EQUAL_INT(1, bus.floorRequests);
    TEST_ASSERT_EQUAL_INT(1, bus.airReads);
    TEST_ASSERT_FALSE(sm.snapshot().floorValid);
    TEST_ASSERT_TRUE(sm.snapshot().airValid);
}

void test_floor_reads_after_conversion_time() {
    FakeBus bus;
    SensorManager sm(bus, 0, kCfg);

    sm.update(0);
    sm.update(749);
    TEST_ASSERT_FALSE(sm.snapshot().floorValid);

    bus.floorTemp = 27.5f;
    sm.update(750);
    TEST_ASSERT_TRUE(sm.snapshot().floorValid);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 27.5f, sm.snapshot().floorTempC);
    TEST_ASSERT_EQUAL_UINT32(750, sm.snapshot().floorSampleMs);
}

void test_poll_intervals() {
    FakeBus bus;
    SensorManager sm(bus, 0, kCfg);

    sm.update(0);
    sm.update(750);
    sm.update(14999);
    TEST_ASSERT_EQUAL_INT(1, bus.airReads);

    sm.update(15000);
    TEST_ASSERT_EQUAL_INT(2, bus.airReads);

    sm.update(29999);
    TEST_ASSERT_EQUAL_INT(1, bus.floorRequests);

    sm.update(30000);
    TEST_ASSERT_EQUAL_INT(2, bus.floorRequests);
}

void test_invalid_floor_values_are_rejected() {
    for (float bad : {85.0f, 0.0f, -127.0f, NAN}) {
        FakeBus bus;
        SensorManager sm(bus, 0, kCfg);
        bus.floorTemp = bad;
        sm.update(0);
        sm.update(750);
        TEST_ASSERT_FALSE_MESSAGE(sm.snapshot().floorValid, "invalid floor temp accepted");
    }
}

void test_invalid_floor_reading_keeps_last_valid() {
    FakeBus bus;
    SensorManager sm(bus, 0, kCfg);

    bus.floorTemp = 27.0f;
    sm.update(0);
    sm.update(750);
    TEST_ASSERT_TRUE(sm.snapshot().floorValid);

    bus.floorTemp = 0.0f;
    sm.update(30000);
    sm.update(30750);

    TEST_ASSERT_TRUE(sm.snapshot().floorValid);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 27.0f, sm.snapshot().floorTempC);
    TEST_ASSERT_EQUAL_UINT32(750, sm.snapshot().floorSampleMs);
}

void test_air_failure_invalidates_and_recovers() {
    FakeBus bus;
    SensorManager sm(bus, 0, kCfg);

    sm.update(0);
    TEST_ASSERT_TRUE(sm.snapshot().airValid);

    bus.airOk = false;
    sm.update(15000);
    TEST_ASSERT_FALSE(sm.snapshot().airValid);

    bus.airOk = true;
    sm.update(30000);
    TEST_ASSERT_TRUE(sm.snapshot().airValid);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 22.0f, sm.snapshot().airTempC);
}

void test_pir_warmup_and_edge_latching() {
    FakeBus bus;
    SensorManager sm(bus, 0, kCfg);

    sm.update(0);

    bus.pir = true;
    sm.update(5000);
    TEST_ASSERT_FALSE(sm.takeMotionStarted());
    TEST_ASSERT_TRUE(sm.snapshot().pirRaw);

    sm.update(10000);
    TEST_ASSERT_FALSE(sm.takeMotionStarted());

    bus.pir = false;
    sm.update(10001);
    TEST_ASSERT_TRUE(sm.takeMotionStopped());

    bus.pir = true;
    sm.update(10002);
    TEST_ASSERT_TRUE(sm.takeMotionStarted());
    TEST_ASSERT_FALSE(sm.takeMotionStarted());
    TEST_ASSERT_EQUAL_UINT32(10002, sm.snapshot().lastMotionMs);

    bus.pir = false;
    sm.update(10003);
    TEST_ASSERT_TRUE(sm.takeMotionStopped());
    TEST_ASSERT_FALSE(sm.takeMotionStopped());
}

void test_pir_high_at_boot_does_not_trigger_after_warmup() {
    FakeBus bus;
    bus.pir = true;
    SensorManager sm(bus, 0, kCfg);

    sm.update(0);
    sm.update(10000);
    TEST_ASSERT_FALSE(sm.takeMotionStarted());
}

}  // namespace

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_first_update_requests_floor_and_reads_air);
    RUN_TEST(test_floor_reads_after_conversion_time);
    RUN_TEST(test_poll_intervals);
    RUN_TEST(test_invalid_floor_values_are_rejected);
    RUN_TEST(test_invalid_floor_reading_keeps_last_valid);
    RUN_TEST(test_air_failure_invalidates_and_recovers);
    RUN_TEST(test_pir_warmup_and_edge_latching);
    RUN_TEST(test_pir_high_at_boot_does_not_trigger_after_warmup);
    return UNITY_END();
}
