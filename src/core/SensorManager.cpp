#include "core/SensorManager.h"

#include "core/SensorLimits.h"

namespace {

constexpr uint32_t kFloorReadyTimeoutMs = 500;
constexpr uint32_t kFloorRetryMs = 2000;

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
        const uint32_t interval = floorRetry_ ? kFloorRetryMs : config_.floorIntervalMs;
        if (!floorRequested_ ||
            static_cast<uint32_t>(now - lastFloorRequestMs_) >= interval) {
            bus_.requestFloorConversion();
            floorRequested_ = true;
            lastFloorRequestMs_ = now;
            floorState_ = FloorState::Waiting;
        }
    } else {
        const uint32_t elapsed = static_cast<uint32_t>(now - lastFloorRequestMs_);
        if (elapsed < config_.floorConversionMs) {
            return;
        }
        // Ждём готовности преобразования, но не дольше таймаута:
        // иначе медленный/отсутствующий датчик навсегда оставит нас в Waiting.
        if (!bus_.isFloorConversionReady() &&
            elapsed < config_.floorConversionMs + kFloorReadyTimeoutMs) {
            return;
        }
        const float tempC = bus_.readFloorTempC();
        snapshot_.floorLastReadC = tempC;
        if (argon::isValidFloorTemp(tempC)) {
            snapshot_.floorTempC = tempC;
            snapshot_.floorSampleMs = now;
            snapshot_.floorValid = true;
            floorRetry_ = false;
        } else {
            // Невалидное чтение сразу помечает данные недостоверными:
            // пол не должен греться по устаревшему значению.
            snapshot_.floorValid = false;
            floorRetry_ = true;
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
