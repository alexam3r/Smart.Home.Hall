#include "core/SensorManager.h"

#include <cmath>

namespace {

bool isValidFloorTemp(float tempC) {
    // DS18B20: -127 — нет устройства, 85 — незавершённое преобразование;
    // 0 °C в жилом помещении считаем ошибкой (решение владельца).
    return !std::isnan(tempC) && tempC > -120.0f && tempC != 0.0f && tempC != 85.0f;
}

}  // namespace

SensorManager::SensorManager(ISensorBus& bus, uint32_t bootMs, SensorConfig config)
    : bus_(bus), config_(config), bootMs_(bootMs) {}

void SensorManager::update(uint32_t now) {
    const bool pir = bus_.readPir();
    snapshot_.pirRaw = pir;

    if (!pirInitialized_) {
        lastPir_ = pir;
        pirInitialized_ = true;
    } else if (pir != lastPir_) {
        const bool warm = static_cast<uint32_t>(now - bootMs_) >= config_.pirWarmupMs;
        if (warm) {
            if (pir) {
                snapshot_.lastMotionMs = now;
                motionStarted_ = true;
            } else {
                motionStopped_ = true;
            }
        }
        lastPir_ = pir;
    }

    if (!airInitialized_ || static_cast<uint32_t>(now - lastAirMs_) >= config_.airIntervalMs) {
        float tempC = 0.0f;
        float humPct = 0.0f;
        const bool ok = bus_.readAir(tempC, humPct);
        airInitialized_ = true;
        lastAirMs_ = now;
        if (ok) {
            snapshot_.airTempC = tempC;
            snapshot_.airHumPct = humPct;
            snapshot_.airValid = true;
        } else {
            snapshot_.airValid = false;
        }
    }

    if (floorState_ == FloorState::Idle) {
        if (!floorRequested_ ||
            static_cast<uint32_t>(now - lastFloorRequestMs_) >= config_.floorIntervalMs) {
            bus_.requestFloorConversion();
            floorRequested_ = true;
            lastFloorRequestMs_ = now;
            floorState_ = FloorState::Waiting;
        }
    } else if (static_cast<uint32_t>(now - lastFloorRequestMs_) >= config_.floorConversionMs) {
        const float tempC = bus_.readFloorTempC();
        if (isValidFloorTemp(tempC)) {
            snapshot_.floorTempC = tempC;
            snapshot_.floorSampleMs = now;
            snapshot_.floorValid = true;
        }
        floorState_ = FloorState::Idle;
    }
}

bool SensorManager::takeMotionStarted() {
    const bool value = motionStarted_;
    motionStarted_ = false;
    return value;
}

bool SensorManager::takeMotionStopped() {
    const bool value = motionStopped_;
    motionStopped_ = false;
    return value;
}
