#pragma once

#include <cstdint>

class IPwmOutput {
public:
    virtual ~IPwmOutput() = default;
    virtual void writeDuty(uint16_t duty) = 0;
};

class ILightObserver {
public:
    virtual ~ILightObserver() = default;
    virtual void onLightStateChanged(bool on) = 0;
    virtual void onBrightnessSettled(uint8_t percent) = 0;
};

struct LightConfig {
    uint32_t fadeMs;
    uint32_t stepMs;
    uint32_t motionTimeoutMs;
    uint32_t safetyTimeoutMs;
    uint8_t defaultBrightness;
    uint16_t pwmMax;
};

class LightController {
public:
    LightController(IPwmOutput& pwm, ILightObserver& observer, LightConfig config, uint32_t now);

    void setPower(bool on, uint32_t now);
    void setBrightness(uint8_t percent, uint32_t now);
    void setMotionMode(bool on, uint32_t now);
    void onMotion(uint32_t now);
    void update(uint32_t now);

    bool isOn() const { return on_; }
    bool motionMode() const { return motionMode_; }
    uint8_t brightness() const { return userBrightness_; }
    bool isFading() const { return fading_; }
    uint32_t secondsSinceActivity(uint32_t now) const;

private:
    void startFade(float target, uint32_t now);
    void applyAutoOff(uint32_t now);
    uint16_t dutyFor(float percent) const;
    uint32_t timeoutMs() const;

    IPwmOutput& pwm_;
    ILightObserver& observer_;
    LightConfig config_;

    bool on_ = false;
    bool motionMode_ = false;
    uint8_t userBrightness_;

    float current_ = 0.0f;
    float start_ = 0.0f;
    float target_ = 0.0f;
    uint32_t fadeStartMs_ = 0;
    uint32_t lastStepMs_ = 0;
    uint32_t lastActivityMs_ = 0;
    bool fading_ = false;
};
