#include <unity.h>

#include <cmath>

#include "core/FloorThermostat.h"

namespace {

constexpr FloorConfig kCfg{0.5f, 1000, 2000, 3000, 30.0f};
constexpr FloorConfig kLong{0.5f, 10000, 20000, 3000, 30.0f};

class FakeRelay : public IRelayOutput {
public:
    bool on = false;
    int changes = 0;
    void writeRelay(bool value) override {
        if (value != on) {
            on = value;
            ++changes;
        }
    }
};

class FakeObserver : public IFloorObserver {
public:
    bool master = false;
    bool heat = false;
    int masterChanges = 0;
    int heatChanges = 0;

    void onMasterChanged(bool value) override {
        master = value;
        ++masterChanges;
    }
    void onHeatChanged(bool value) override {
        heat = value;
        ++heatChanges;
    }
};

void test_initial_state_is_off() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    TEST_ASSERT_FALSE(floor.master());
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_EQUAL_FLOAT(30.0f, floor.target());
    TEST_ASSERT_FALSE(relay.on);
    TEST_ASSERT_EQUAL_INT(0, observer.masterChanges);
    TEST_ASSERT_EQUAL_INT(0, observer.heatChanges);
}

void test_hysteresis_holds_between_bounds() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    TEST_ASSERT_EQUAL_INT(1, observer.masterChanges);

    floor.onSample(29.4f, 0);
    TEST_ASSERT_TRUE(floor.heat());
    TEST_ASSERT_TRUE(relay.on);

    floor.onSample(29.6f, 0);
    TEST_ASSERT_TRUE(floor.heat());

    floor.onSample(30.6f, 0);
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_FALSE(relay.on);

    floor.onSample(30.4f, 0);
    TEST_ASSERT_FALSE(floor.heat());

    TEST_ASSERT_EQUAL_INT(2, observer.heatChanges);
    TEST_ASSERT_EQUAL_INT(2, relay.changes);
}

void test_target_change_recomputes_immediately() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.onSample(30.0f, 0);
    TEST_ASSERT_FALSE(floor.heat());

    floor.setTarget(31.0f, 0);
    TEST_ASSERT_TRUE(floor.heat());
    TEST_ASSERT_TRUE(relay.on);
}

void test_motion_in_motion_mode_turns_master_on() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMotionMode(true, 0);
    floor.onMotion(100);
    TEST_ASSERT_TRUE(floor.master());
    TEST_ASSERT_EQUAL_INT(1, observer.masterChanges);

    floor.onSample(29.0f, 200);
    TEST_ASSERT_TRUE(floor.heat());
}

void test_motion_without_motion_mode_does_not_turn_master_on() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.onMotion(100);
    TEST_ASSERT_FALSE(floor.master());
}

void test_master_times_out_after_motion_window() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMotionMode(true, 0);
    floor.setMaster(true, 0);
    floor.onSample(28.0f, 0);
    TEST_ASSERT_TRUE(floor.heat());

    floor.update(999);
    TEST_ASSERT_TRUE(floor.master());
    TEST_ASSERT_TRUE(floor.heat());

    floor.update(1000);
    TEST_ASSERT_FALSE(floor.master());
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_FALSE(relay.on);
    TEST_ASSERT_EQUAL_INT(2, observer.masterChanges);
}

void test_safety_timeout_without_motion_mode() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.update(1999);
    TEST_ASSERT_TRUE(floor.master());

    floor.update(2000);
    TEST_ASSERT_FALSE(floor.master());
}

void test_floor_command_resets_timer() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.setTarget(30.0f, 900);
    floor.update(1899);
    TEST_ASSERT_TRUE(floor.master());

    floor.update(2900);
    TEST_ASSERT_FALSE(floor.master());
}

void test_stale_sample_turns_heat_off_but_master_stays_on() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kLong, 0);

    floor.setMaster(true, 0);
    floor.onSample(28.0f, 0);
    TEST_ASSERT_TRUE(floor.heat());

    floor.update(2999);
    TEST_ASSERT_TRUE(floor.heat());

    floor.update(3000);
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_TRUE(floor.master());

    floor.onSample(28.0f, 3000);
    TEST_ASSERT_TRUE(floor.heat());
}

void test_master_off_turns_heat_off() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.onSample(28.0f, 0);
    TEST_ASSERT_TRUE(relay.on);

    floor.setMaster(false, 0);
    TEST_ASSERT_FALSE(floor.master());
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_FALSE(relay.on);
    TEST_ASSERT_EQUAL_INT(2, observer.masterChanges);
    TEST_ASSERT_EQUAL_INT(2, observer.heatChanges);
}

void test_invalid_samples_are_ignored() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.onSample(85.0f, 0);
    floor.onSample(0.0f, 0);
    floor.onSample(-127.0f, 0);
    floor.onSample(NAN, 0);

    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_FALSE(relay.on);
}

void test_temperature_below_five_is_ignored() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.onSample(4.9f, 0);
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_FALSE(relay.on);
}

void test_sensor_invalid_turns_heat_off_but_keeps_master() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.setMaster(true, 0);
    floor.onSample(28.0f, 0);
    TEST_ASSERT_TRUE(floor.heat());

    floor.onSensorInvalid(100);
    TEST_ASSERT_FALSE(floor.heat());
    TEST_ASSERT_TRUE(floor.master());
    TEST_ASSERT_FALSE(relay.on);

    floor.onSample(28.0f, 200);
    TEST_ASSERT_TRUE(floor.heat());
}

void test_master_on_uses_cached_sample() {
    FakeRelay relay;
    FakeObserver observer;
    FloorThermostat floor(relay, observer, kCfg, 0);

    floor.onSample(28.0f, 0);
    TEST_ASSERT_FALSE(floor.heat());

    floor.setMaster(true, 0);
    TEST_ASSERT_TRUE(floor.heat());
}

}  // namespace

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_off);
    RUN_TEST(test_hysteresis_holds_between_bounds);
    RUN_TEST(test_target_change_recomputes_immediately);
    RUN_TEST(test_motion_in_motion_mode_turns_master_on);
    RUN_TEST(test_motion_without_motion_mode_does_not_turn_master_on);
    RUN_TEST(test_master_times_out_after_motion_window);
    RUN_TEST(test_safety_timeout_without_motion_mode);
    RUN_TEST(test_floor_command_resets_timer);
    RUN_TEST(test_stale_sample_turns_heat_off_but_master_stays_on);
    RUN_TEST(test_master_off_turns_heat_off);
    RUN_TEST(test_invalid_samples_are_ignored);
    RUN_TEST(test_temperature_below_five_is_ignored);
    RUN_TEST(test_sensor_invalid_turns_heat_off_but_keeps_master);
    RUN_TEST(test_master_on_uses_cached_sample);
    return UNITY_END();
}
