#include "core/FloorThermostat.h"

#include <cmath>

namespace {

bool isValidFloorTemp(float tempC) {
    return !std::isnan(tempC) && tempC > -120.0f && tempC != 0.0f && tempC != 85.0f;
}

}  // namespace

FloorThermostat::FloorThermostat(IRelayOutput& relay, IFloorObserver& observer,
                                 FloorConfig config, uint32_t now)
    : relay_(relay),
      observer_(observer),
      config_(config),
      target_(config.defaultTargetC),
      lastActivityMs_(now) {}

void FloorThermostat::setMaster(bool on, uint32_t now) {
    lastActivityMs_ = now;
    if (master_ != on) {
        master_ = on;
        observer_.onMasterChanged(on);
    }
    recomputeHeat(now);
}

void FloorThermostat::setMotionMode(bool on, uint32_t now) {
    motionMode_ = on;
    lastActivityMs_ = now;
    recomputeHeat(now);
}

void FloorThermostat::setTarget(float targetC, uint32_t now) {
    target_ = targetC;
    lastActivityMs_ = now;
    recomputeHeat(now);
}

void FloorThermostat::onMotion(uint32_t now) {
    lastActivityMs_ = now;
    if (motionMode_ && !master_) {
        master_ = true;
        observer_.onMasterChanged(true);
    }
    recomputeHeat(now);
}

void FloorThermostat::onSample(float floorTempC, uint32_t now) {
    if (!isValidFloorTemp(floorTempC)) {
        return;
    }
    floorTempC_ = floorTempC;
    hasSample_ = true;
    lastValidSampleMs_ = now;
    recomputeHeat(now);
}

void FloorThermostat::update(uint32_t now) {
    if (master_ && static_cast<uint32_t>(now - lastActivityMs_) >= windowTimeoutMs()) {
        master_ = false;
        observer_.onMasterChanged(false);
    }
    recomputeHeat(now);
}

void FloorThermostat::recomputeHeat(uint32_t now) {
    bool next = heat_;

    const bool windowActive = static_cast<uint32_t>(now - lastActivityMs_) < windowTimeoutMs();
    const bool sampleFresh =
        hasSample_ && static_cast<uint32_t>(now - lastValidSampleMs_) < config_.staleTimeoutMs;

    if (!master_ || !windowActive || !sampleFresh) {
        next = false;
    } else if (floorTempC_ < target_ - config_.hysteresisC) {
        next = true;
    } else if (floorTempC_ > target_ + config_.hysteresisC) {
        next = false;
    }

    if (next != heat_) {
        heat_ = next;
        relay_.writeRelay(heat_);
        observer_.onHeatChanged(heat_);
    }
}

uint32_t FloorThermostat::windowTimeoutMs() const {
    return motionMode_ ? config_.motionTimeoutMs : config_.safetyTimeoutMs;
}
