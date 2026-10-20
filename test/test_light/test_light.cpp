#include <unity.h>

#include "core/Cie1931.h"
#include "core/LightController.h"

namespace {

constexpr LightConfig kCfg{2000, 20, 60000, 3600000, 100, 1023};
constexpr LightConfig kFast{2000, 20, 100, 200, 100, 1023};

class FakePwm : public IPwmOutput {
public:
    uint16_t duty = 0;
    int writes = 0;
    void writeDuty(uint16_t value) override {
        duty = value;
        ++writes;
    }
};

class FakeObserver : public ILightObserver {
public:
    bool on = false;
    int stateChanges = 0;
    int settledCount = 0;
    uint8_t settledPercent = 0;

    void onLightStateChanged(bool value) override {
        on = value;
        ++stateChanges;
    }
    void onBrightnessSettled(uint8_t percent) override {
        settledPercent = percent;
        ++settledCount;
    }
};

void test_initial_state_is_off_at_zero() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    TEST_ASSERT_FALSE(light.isOn());
    TEST_ASSERT_EQUAL_UINT8(100, light.brightness());
    TEST_ASSERT_EQUAL_UINT16(0, pwm.duty);
    TEST_ASSERT_EQUAL_INT(0, observer.stateChanges);
}

void test_power_on_fades_to_full_in_two_seconds() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.setPower(true, 0);
    TEST_ASSERT_TRUE(light.isOn());
    TEST_ASSERT_EQUAL_INT(1, observer.stateChanges);
    TEST_ASSERT_TRUE(observer.on);

    light.update(1000);
    TEST_ASSERT_EQUAL_UINT16(188, pwm.duty);

    light.update(2000);
    TEST_ASSERT_EQUAL_UINT16(1023, pwm.duty);
    TEST_ASSERT_EQUAL_INT(1, observer.settledCount);
    TEST_ASSERT_EQUAL_UINT8(100, observer.settledPercent);
}

void test_brightness_change_fades_from_current_level() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.setPower(true, 0);
    light.update(2000);

    light.setBrightness(30, 2000);
    light.update(3000);
    TEST_ASSERT_EQUAL_UINT16(348, pwm.duty);

    light.update(4000);
    TEST_ASSERT_EQUAL_UINT16(63, pwm.duty);
    TEST_ASSERT_EQUAL_INT(2, observer.settledCount);
    TEST_ASSERT_EQUAL_UINT8(30, observer.settledPercent);
}

void test_reverse_fades_from_current_point() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.setPower(true, 0);
    light.update(1000);

    light.setPower(false, 1000);
    TEST_ASSERT_FALSE(light.isOn());
    TEST_ASSERT_EQUAL_INT(2, observer.stateChanges);

    light.update(2000);
    TEST_ASSERT_EQUAL_UINT16(45, pwm.duty);

    light.update(3000);
    TEST_ASSERT_EQUAL_UINT16(0, pwm.duty);
    TEST_ASSERT_EQUAL_UINT8(100, observer.settledPercent);
}

void test_ha_brightness_then_on_and_reset_only_on_off() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.setBrightness(30, 0);
    TEST_ASSERT_FALSE(light.isOn());

    light.setPower(true, 0);
    light.update(2000);
    TEST_ASSERT_EQUAL_UINT16(63, pwm.duty);
    TEST_ASSERT_EQUAL_UINT8(30, observer.settledPercent);

    light.setPower(false, 2000);
    TEST_ASSERT_EQUAL_UINT8(100, light.brightness());
    light.update(4000);

    light.setPower(true, 4000);
    light.update(6000);
    TEST_ASSERT_EQUAL_UINT16(1023, pwm.duty);
    TEST_ASSERT_EQUAL_UINT8(100, observer.settledPercent);
}

void test_motion_mode_timeout() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kFast, 0);

    light.setMotionMode(true, 0);
    light.setPower(true, 0);
    light.update(99);
    TEST_ASSERT_TRUE(light.isOn());

    light.update(100);
    TEST_ASSERT_FALSE(light.isOn());
    TEST_ASSERT_EQUAL_INT(2, observer.stateChanges);
    TEST_ASSERT_EQUAL_UINT32(5, light.secondsSinceActivity(5000));
}

void test_safety_timeout_without_motion_mode() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kFast, 0);

    light.setPower(true, 0);
    light.update(199);
    TEST_ASSERT_TRUE(light.isOn());

    light.update(200);
    TEST_ASSERT_FALSE(light.isOn());
}

void test_any_light_command_resets_timer() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kFast, 0);

    light.setPower(true, 0);
    light.setMotionMode(true, 0);
    light.update(90);
    TEST_ASSERT_TRUE(light.isOn());

    light.setBrightness(50, 90);
    light.update(189);
    TEST_ASSERT_TRUE(light.isOn());

    light.update(190);
    TEST_ASSERT_FALSE(light.isOn());
}

void test_late_update_catches_up_without_overshoot() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.setPower(true, 0);
    light.update(750);
    TEST_ASSERT_EQUAL_UINT16(97, pwm.duty);
    TEST_ASSERT_TRUE(light.isOn());
}

void test_stale_loop_time_does_not_skip_reverse_fade() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.setPower(true, 0);
    light.update(1000);
    TEST_ASSERT_EQUAL_UINT16(188, pwm.duty);

    // Команда пришла из MQTT-callback с более поздним millis(),
    // а loop() ещё передаёт старый now.
    light.setPower(false, 1100);
    light.update(1000);
    TEST_ASSERT_FALSE(light.isOn());
    TEST_ASSERT_TRUE(light.isFading());
    TEST_ASSERT_EQUAL_UINT16(188, pwm.duty);

    light.update(2100);
    TEST_ASSERT_EQUAL_UINT16(45, pwm.duty);
}

void test_motion_turns_light_on_only_in_motion_mode() {
    FakePwm pwm;
    FakeObserver observer;
    LightController light(pwm, observer, kCfg, 0);

    light.onMotion(1000);
    TEST_ASSERT_FALSE(light.isOn());

    light.setMotionMode(true, 1000);
    light.onMotion(2000);
    TEST_ASSERT_TRUE(light.isOn());
    TEST_ASSERT_EQUAL_INT(1, observer.stateChanges);
}

}  // namespace

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_off_at_zero);
    RUN_TEST(test_power_on_fades_to_full_in_two_seconds);
    RUN_TEST(test_brightness_change_fades_from_current_level);
    RUN_TEST(test_reverse_fades_from_current_point);
    RUN_TEST(test_ha_brightness_then_on_and_reset_only_on_off);
    RUN_TEST(test_motion_mode_timeout);
    RUN_TEST(test_safety_timeout_without_motion_mode);
    RUN_TEST(test_any_light_command_resets_timer);
    RUN_TEST(test_late_update_catches_up_without_overshoot);
    RUN_TEST(test_stale_loop_time_does_not_skip_reverse_fade);
    RUN_TEST(test_motion_turns_light_on_only_in_motion_mode);
    return UNITY_END();
}
