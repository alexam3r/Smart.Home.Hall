#include "core/LightController.h"

#include "core/Cie1931.h"

LightController::LightController(IPwmOutput& pwm, ILightObserver& observer,
                                 LightConfig config, uint32_t now)
    : pwm_(pwm),
      observer_(observer),
      config_(config),
      userBrightness_(config.defaultBrightness),
      fadeStartMs_(now),
      lastStepMs_(now),
      lastActivityMs_(now) {}

void LightController::setPower(bool on, uint32_t now) {
    lastActivityMs_ = now;

    if (on != on_) {
        on_ = on;
        observer_.onLightStateChanged(on);
    }

    if (!on) {
        userBrightness_ = config_.defaultBrightness;
        startFade(0.0f, now);
    } else {
        startFade(static_cast<float>(userBrightness_), now);
    }
}

void LightController::setBrightness(uint8_t percent, uint32_t now) {
    if (percent < 1) {
        percent = 1;
    } else if (percent > 100) {
        percent = 100;
    }

    userBrightness_ = percent;
    lastActivityMs_ = now;

    if (on_) {
        startFade(static_cast<float>(percent), now);
    }
}

void LightController::setMotionMode(bool on, uint32_t now) {
    motionMode_ = on;
    lastActivityMs_ = now;
}

void LightController::onMotion(uint32_t now) {
    lastActivityMs_ = now;
    if (motionMode_ && !on_) {
        setPower(true, now);
    }
}

void LightController::update(uint32_t now) {
    const int32_t idle = static_cast<int32_t>(now - lastActivityMs_);
    if (on_ && idle >= static_cast<int32_t>(timeoutMs())) {
        applyAutoOff(now);
    }

    if (!fading_) {
        return;
    }

    const int32_t elapsed = static_cast<int32_t>(now - fadeStartMs_);
    if (elapsed <= 0) {
        // Команда пришла из callback с более поздним millis(), чем now в loop().
        // Нельзя трактовать это как «фейд завершён».
        return;
    }

    const float progress = config_.fadeMs == 0
                               ? 1.0f
                               : static_cast<float>(elapsed) / static_cast<float>(config_.fadeMs);

    if (progress >= 1.0f) {
        current_ = target_;
        fading_ = false;
        pwm_.writeDuty(dutyFor(current_));
        observer_.onBrightnessSettled(userBrightness_);
        return;
    }

    current_ = start_ + (target_ - start_) * progress;

    if (static_cast<int32_t>(now - lastStepMs_) >= static_cast<int32_t>(config_.stepMs)) {
        pwm_.writeDuty(dutyFor(current_));
        lastStepMs_ = now;
    }
}

uint32_t LightController::secondsSinceActivity(uint32_t now) const {
    const int32_t elapsed = static_cast<int32_t>(now - lastActivityMs_);
    return elapsed > 0 ? static_cast<uint32_t>(elapsed) / 1000u : 0;
}

void LightController::startFade(float target, uint32_t now) {
    if (fading_ && target_ == target) {
        return;
    }

    start_ = current_;
    target_ = target;
    fadeStartMs_ = now;
    lastStepMs_ = now;
    fading_ = current_ != target_;
}

void LightController::applyAutoOff(uint32_t now) {
    on_ = false;
    observer_.onLightStateChanged(false);
    // Как в легаси: авто-выключение сбрасывает пользовательскую яркость,
    // но не таймер бездействия (since_move_s продолжает расти).
    userBrightness_ = config_.defaultBrightness;
    startFade(0.0f, now);
}

uint16_t LightController::dutyFor(float percent) const {
    const uint16_t base = cie1931(static_cast<uint8_t>(percent));
    if (config_.pwmMax == 1023) {
        return base;
    }
    return static_cast<uint16_t>((static_cast<uint32_t>(base) * config_.pwmMax) / 1023u);
}

uint32_t LightController::timeoutMs() const {
    return motionMode_ ? config_.motionTimeoutMs : config_.safetyTimeoutMs;
}
