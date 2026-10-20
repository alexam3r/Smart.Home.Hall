#pragma once

#include <cstdint>

class IRelayOutput {
public:
    virtual ~IRelayOutput() = default;
    virtual void writeRelay(bool on) = 0;
};

class IFloorObserver {
public:
    virtual ~IFloorObserver() = default;
    virtual void onMasterChanged(bool on) = 0;
    virtual void onHeatChanged(bool on) = 0;
};

struct FloorConfig {
    float hysteresisC;
    uint32_t motionTimeoutMs;
    uint32_t safetyTimeoutMs;
    uint32_t staleTimeoutMs;
    float defaultTargetC;
};

class FloorThermostat {
public:
    FloorThermostat(IRelayOutput& relay, IFloorObserver& observer, FloorConfig config, uint32_t now);

    void setMaster(bool on, uint32_t now);
    void setMotionMode(bool on, uint32_t now);
    void setTarget(float targetC, uint32_t now);
    void onMotion(uint32_t now);
    void onSample(float floorTempC, uint32_t now);
    void update(uint32_t now);

    bool master() const { return master_; }
    bool heat() const { return heat_; }
    bool motionMode() const { return motionMode_; }
    float target() const { return target_; }

private:
    void recomputeHeat(uint32_t now);
    uint32_t windowTimeoutMs() const;

    IRelayOutput& relay_;
    IFloorObserver& observer_;
    FloorConfig config_;

    bool master_ = false;
    bool motionMode_ = false;
    bool heat_ = false;
    float target_;

    bool hasSample_ = false;
    float floorTempC_ = 0.0f;
    uint32_t lastValidSampleMs_ = 0;
    uint32_t lastActivityMs_ = 0;
};
