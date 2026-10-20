#pragma once

#include <cstdint>

// Абстракция над датчиками: позволяет тестировать автомат на хосте.
class ISensorBus {
public:
    virtual ~ISensorBus() = default;

    // Запускает преобразование DS18B20 и сразу возвращает управление.
    virtual void requestFloorConversion() = 0;
    // Готово ли преобразование DS18B20 (для асинхронного ожидания).
    virtual bool isFloorConversionReady() = 0;
    // Читает результат последнего преобразования (может вернуть 85/0/-127).
    virtual float readFloorTempC() = 0;
    // Один обмен с AM2320: температура и влажность. false — ошибка шины.
    virtual bool readAir(float& tempC, float& humPct) = 0;
    virtual bool readPir() = 0;
};

struct SensorConfig {
    uint32_t floorIntervalMs;
    uint32_t floorConversionMs;
    uint32_t airIntervalMs;
    uint32_t pirWarmupMs;
};

struct SensorSnapshot {
    bool floorValid = false;
    float floorTempC = 0.0f;
    float floorLastReadC = 0.0f;
    uint32_t floorSampleMs = 0;

    bool airValid = false;
    float airTempC = 0.0f;
    float airHumPct = 0.0f;

    bool pirRaw = false;
    uint32_t lastMotionMs = 0;
};

class SensorManager {
public:
    SensorManager(ISensorBus& bus, uint32_t bootMs, SensorConfig config);

    void update(uint32_t now);
    const SensorSnapshot& snapshot() const { return snapshot_; }

    // Фронты PIR забираются один раз.
    bool takeMotionStarted();
    bool takeMotionStopped();

private:
    enum class FloorState { Idle, Waiting };

    ISensorBus& bus_;
    SensorConfig config_;
    uint32_t bootMs_;

    SensorSnapshot snapshot_{};

    FloorState floorState_ = FloorState::Idle;
    bool floorRequested_ = false;
    bool floorRetry_ = false;
    uint32_t lastFloorRequestMs_ = 0;

    bool airInitialized_ = false;
    uint32_t lastAirMs_ = 0;

    bool pirInitialized_ = false;
    bool lastPir_ = false;
    bool motionStarted_ = false;
    bool motionStopped_ = false;
};
