# План реализации рефакторинга Argon

> **For agentic workers:** REQUIRED SUB-SKILL: Use subagent-driven-development (recommended) or executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Переписать монолитную прошивку `docs/main.cpp` в модульную неблокирующую прошивку для Wemos D1 Mini, сохранив аппаратную часть, MQTT-контракт и бизнес-логику.

**Architecture:** Чистая логика (свет, пол, датчики, телеметрия) выносится в `src/core/` без зависимостей от Arduino и тестируется на хосте через PlatformIO `native`. Аппаратные адаптеры в `src/hal/` реализуют узкие интерфейсы (`IPwmOutput`, `IRelayOutput`, `ISensorBus`). `NetworkManager` отвечает за Wi-Fi/MQTT и вызывает контроллеры через `IMqttCommandSink`; `main.cpp` только связывает модули. Все ожидания — по `millis()`, без `delay()` в `loop()`.

**Tech Stack:** C++17, PlatformIO (env `d1_mini` + `native`), ESP8266 Arduino core 3.3.0, PubSubClient 2.8, ArduinoJson 6.21, OneWire 2.3.8, DallasTemperature 3.11, Adafruit AM2320 1.2.5, Unity.

**Spec:** `PRD.md` (§1–6), `docs/freeze-analysis.md` (§1–9).

## Global Constraints

- MCU: Wemos D1 Mini (ESP8266), платформа `espressif8266`, Arduino framework.
- Пины: D7/GPIO13 — ШИМ ленты, D8/GPIO15 — SSR пола, D5/GPIO14 — DS18B20, D6/GPIO12 — PIR, D1/GPIO5 — SCL, D2/GPIO4 — SDA. Не менять.
- ШИМ: 4000 Гц, диапазон 0…1023, прямая логика.
- Кривая CIE1931; переход 2 с, обновление каждые 20 мс, реверс с текущей яркости.
- Тайминги: PIR-прогрев 10 с; свет 60 с / 3600 с; пол 3600 с / 43200 с; гистерезис ±0.5 °C; fail-safe пола 90 с; AM2320 15 с; DS18B20 30 с + 850 мс на преобразование; телеметрия 10 с; reconnect 5 с; ребут при потере связи 5 мин.
- MQTT: база `home/hall`, client ID `ESP8266_Smart_Hall`, топики и retain — строго по `README.md`.
- Порядок ключей JSON телеметрии: `light_state, light_move_state, floor_state, floor_move_state, floor_heat_state, floor_temp, air_temp, air_hum, pir_raw, since_move_s, uptime, heap`.
- Секреты — только `include/Secrets.h` (в `.gitignore`); в репозитории — `include/Secrets.h.example`.
- `loop()` не блокировать: без `delay()`, без ожидания преобразования DS18B20, без синхронных сенсорных чтений длиннее ~15 мс.
- C++17, отступ 4 пробела, UTF-8, LF; комментарии по-русски.
- После каждого блока: `pio run`, `pio test -e native`, `graphify update .`, коммит, отчёт и **пауза** до команды человека.

## Review Focus

1. **Пропущенный тик во время фейда.** Если `loop()` задержался (например, на 750 мс), времязависимый фейдер должен прыгнуть на правильную яркость, не перелететь цель и не «зависнуть» — особенно при реверсе посередине.
2. **Невалидные/устаревшие данные датчиков.** `-127`, `85`, `0 °C`, `NaN` не должны попадать в телеметрию как значения; устаревший замер пола дольше 90 с обязан выключить нагрев, не трогая мастер-режим.
3. **Границы гистерезиса.** Ровно `target ± 0.5 °C` не должны вызывать дребезг реле; смена уставки/мастера должна немедленно пересчитывать реле по последнему валидному замеру.
4. **Порядок команд Home Assistant.** `brightness` → `ON` должен включать на заданной яркости; `OFF` сбрасывает пользовательскую яркость в 100, и только `OFF`.
5. **Телеметрия.** Порядок ключей, `null` для ошибок, отсутствие обрезания JSON в буфере 512 байт при максимальных значениях.

---

## File Structure

```text
platformio.ini                     env d1_mini + native, зависимости
include/
  Config.h                         пины, тайминги, константы (без Arduino)
  MqttTopics.h                     строковые константы топиков
  Secrets.h.example                шаблон секретов
  core/
    Cie1931.h                      uint16_t cie1931(uint8_t percent)
    Cie1931Table.h                 сгенерированная таблица 0…100 (PROGMEM)
    LightController.h              логика света, интерфейсы PWM/наблюдателя
    FloorThermostat.h              логика пола, интерфейсы реле/наблюдателя
    SensorManager.h                асинхронный опрос, интерфейс ISensorBus
    TelemetryFormatter.h           формирование JSON
  hal/
    ArduinoPwm.h                   IPwmOutput на analogWrite
    ArduinoRelay.h                 IRelayOutput на digitalWrite
    ArduinoSensors.h               ISensorBus на OneWire/Dallas/AM2320
  NetworkManager.h                 Wi-Fi/MQTT, IMqttCommandSink
src/
  main.cpp                         сборка модулей, loop без блокировок
  core/
    Cie1931.cpp
    LightController.cpp
    FloorThermostat.cpp
    SensorManager.cpp
    TelemetryFormatter.cpp
  hal/
    ArduinoPwm.cpp
    ArduinoRelay.cpp
    ArduinoSensors.cpp
  NetworkManager.cpp
test/
  test_smoke/test_smoke.cpp
  test_sensors/test_sensors.cpp
  test_cie/test_cie.cpp
  test_light/test_light.cpp
  test_floor/test_floor.cpp
  test_telemetry/test_telemetry.cpp
tools/
  gen_cie_lut.py                   генератор таблицы CIE1931
```

---

# Блок 1. Инфраструктура

## Task 1.1: Native-тесты и Config

**Files:**
- Modify: `platformio.ini`
- Create: `include/Config.h`
- Create: `include/MqttTopics.h`
- Create: `test/test_smoke/test_smoke.cpp`

**Interfaces:**
- Produces: `namespace cfg` с константами пинов/таймингов; `namespace mqtt` с константами топиков. Оба заголовка не включают Arduino.

- [ ] **Step 1: Написать падающий smoke-тест**

```cpp
#include <unity.h>
#include "Config.h"
#include "MqttTopics.h"

void test_constants() {
    TEST_ASSERT_EQUAL_UINT8(13, cfg::PIN_PWM_LED);
    TEST_ASSERT_EQUAL_UINT16(1023, cfg::PWM_RANGE);
    TEST_ASSERT_EQUAL_UINT32(30000, cfg::FLOOR_INTERVAL_MS);
    TEST_ASSERT_EQUAL_STRING("home/hall/light/light", mqtt::LIGHT_CMD);
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_constants);
    return UNITY_END();
}
```

- [ ] **Step 2: Добавить env `native` и убедиться, что тест падает**

```ini
[env:native]
platform = native
test_framework = unity
test_build_src = yes
build_src_filter = +<core/>
build_flags = -std=gnu++17 -Iinclude
lib_deps = bblanchon/ArduinoJson@^6.21.3
```

Run: `pio test -e native` — Expected: FAIL (нет `Config.h`).

- [ ] **Step 3: Создать `include/Config.h` и `include/MqttTopics.h`**

`Config.h` — только `<cstdint>`; пины в GPIO-номерах с комментариями `// D7`:

```cpp
constexpr uint8_t PIN_SCL = 5;          // D1
constexpr uint8_t PIN_SDA = 4;          // D2
constexpr uint8_t PIN_DS18B20 = 14;     // D5
constexpr uint8_t PIN_PIR = 12;         // D6
constexpr uint8_t PIN_PWM_LED = 13;     // D7
constexpr uint8_t PIN_SSR_FLOOR = 15;   // D8

constexpr uint32_t PWM_FREQ_HZ = 4000;
constexpr uint16_t PWM_RANGE = 1023;

constexpr uint32_t LIGHT_FADE_MS = 2000;
constexpr uint32_t LIGHT_STEP_MS = 20;
constexpr uint32_t LIGHT_PIR_TIMEOUT_MS = 60000;
constexpr uint32_t LIGHT_SAFETY_TIMEOUT_MS = 3600000;
constexpr uint8_t  LIGHT_DEFAULT_BRIGHTNESS = 100;

constexpr uint32_t FLOOR_MOTION_TIMEOUT_MS = 3600000;
constexpr uint32_t FLOOR_SAFETY_TIMEOUT_MS = 43200000;
constexpr uint32_t FLOOR_STALE_TIMEOUT_MS = 90000;
constexpr float    FLOOR_HYSTERESIS_C = 0.5f;
constexpr float    FLOOR_DEFAULT_TARGET_C = 30.0f;

constexpr uint32_t PIR_WARMUP_MS = 10000;
constexpr uint32_t FLOOR_INTERVAL_MS = 30000;
constexpr uint32_t FLOOR_CONVERSION_MS = 750;
constexpr uint32_t AIR_INTERVAL_MS = 15000;
constexpr uint32_t TELEMETRY_INTERVAL_MS = 10000;

constexpr uint32_t MQTT_RETRY_MS = 5000;
constexpr uint32_t MQTT_LOST_REBOOT_MS = 300000;
constexpr char     MQTT_CLIENT_ID[] = "ESP8266_Smart_Hall";
constexpr uint16_t TELEMETRY_BUFFER_SIZE = 512;
```

`MqttTopics.h` — `constexpr char NAME[] = "home/hall/..."` для всех команд, состояний и телеметрии.

- [ ] **Step 4: Прогнать тест**

Run: `pio test -e native` — Expected: PASS, 1 test.

- [ ] **Step 5: Commit**

```bash
git add platformio.ini include/Config.h include/MqttTopics.h test/test_smoke
git commit -m "build: native test env и конфигурационные константы"
```

## Task 1.2: Секреты и скелет прошивки

**Files:**
- Create: `include/Secrets.h.example`
- Create: `src/main.cpp`

**Interfaces:**
- Produces: `setup()` инициализирует Serial, ШИМ, пины «всё выключено»; `loop()` пока пуст.

- [ ] **Step 1: Создать шаблон секретов**

```cpp
#pragma once
#define WIFI_SSID "your-ssid"
#define WIFI_PASS "your-password"
#define MQTT_USER "mqtt-user"
#define MQTT_PASS "mqtt-password"
```

- [ ] **Step 2: Создать скелет `src/main.cpp`**

`setup()`: `Serial.begin(115200)`, `analogWriteFreq(cfg::PWM_FREQ_HZ)`, `analogWriteRange(cfg::PWM_RANGE)`, `pinMode`/`digitalWrite` для D7 и D8 (оба выключены), `pinMode(PIN_PIR, INPUT)`, лог `"[SYS] Argon skeleton ready"`. `loop()` пуст.

- [ ] **Step 3: Собрать**

Run: `pio run` — Expected: SUCCESS.

- [ ] **Step 4: Commit**

```bash
git add include/Secrets.h.example src/main.cpp
git commit -m "feat: скелет прошивки и шаблон секретов"
```

## Block 1 gate — STOP

- [ ] `pio run` и `pio test -e native` зелёные; приложить хвост вывода.
- [ ] `graphify update .`; закоммитить `graphify-out/graph.json` и `GRAPH_REPORT.md`.
- [ ] `git commit -m "chore: обновить граф после блока 1"`.
- [ ] Отчёт человеку и **пауза** до команды «продолжай».

---

# Блок 2. Датчики и неблокирующий опрос

## Task 2.1: `SensorManager` — чистый автомат

**Files:**
- Create: `include/core/SensorManager.h`
- Create: `src/core/SensorManager.cpp`
- Create: `test/test_sensors/test_sensors.cpp`

**Interfaces:**
- Produces:
  - `class ISensorBus { virtual void requestFloorConversion() = 0; virtual float readFloorTempC() = 0; virtual bool readAir(float& tempC, float& humPct) = 0; virtual bool readPir() = 0; };`
  - `struct SensorConfig { uint32_t floorIntervalMs, floorConversionMs, airIntervalMs, pirWarmupMs; };`
  - `struct SensorSnapshot { bool floorValid; float floorTempC; uint32_t floorSampleMs; bool airValid; float airTempC; float airHumPct; bool pirRaw; uint32_t lastMotionMs; };`
  - `class SensorManager { SensorManager(ISensorBus&, uint32_t bootMs, SensorConfig); void update(uint32_t now); const SensorSnapshot& snapshot() const; bool takeMotionStarted(); bool takeMotionStopped(); };`
- Consumes: ничего из предыдущих задач, кроме типов.

- [ ] **Step 1: Написать падающие тесты**

Fake bus записывает количество запросов и отдаёт заданные значения. Обязательные кейсы:

```cpp
// t=0: первый update — запрос DS18B20 и чтение AM2320
sm.update(0);
TEST_ASSERT_EQUAL(1, bus.floorRequests);
TEST_ASSERT_EQUAL(1, bus.airReads);

// через 749 мс чтения ещё нет, через 750 — есть
sm.update(749); TEST_ASSERT_FALSE(sm.snapshot().floorValid);
bus.floorTemp = 27.5f; sm.update(750);
TEST_ASSERT_TRUE(sm.snapshot().floorValid);
TEST_ASSERT_FLOAT_WITHIN(0.01f, 27.5f, sm.snapshot().floorTempC);

// следующий запрос — на 30000 мс, AM2320 — на 15000
sm.update(15000); TEST_ASSERT_EQUAL(2, bus.airReads);
sm.update(30000); TEST_ASSERT_EQUAL(2, bus.floorRequests);

// ошибки -127/85/0/NaN не становятся валидным значением
// после валидного замера невалидный не затирает последний валидный
// PIR до 10000 мс игнорируется, после — даёт фронт
// takeMotionStarted()/takeMotionStopped() очищают флаг
```

- [ ] **Step 2: Убедиться, что тесты падают**

Run: `pio test -e native -f test_sensors` — Expected: FAIL (нет `SensorManager`).

- [ ] **Step 3: Реализовать автомат**

Правила:
- PIR опрашивается каждый `update`; фронты фиксируются, но до `bootMs + pirWarmupMs` флаги не выставляются.
- AM2320: первый опрос на первом `update`, далее `(now - lastAir) >= airIntervalMs`.
- DS18B20: первый запрос на первом `update`; состояние `WAIT`; через `floorConversionMs` — чтение и возврат в `IDLE`; следующий запрос через `floorIntervalMs` от предыдущего запроса.
- Валидность пола: `!isnan(t) && t > -120.0f && t != 0.0f && t != 85.0f`. При невалидном чтении последнее валидное значение и `floorSampleMs` сохраняются.
- Валидность воздуха: при неудачном чтении `airValid = false` (в телеметрии `null`, как в легаси); при следующем успешном чтении — снова `true`.
- Все интервалы — только `(now - last) >= interval` с `uint32_t`.

- [ ] **Step 4: Прогнать тесты**

Run: `pio test -e native -f test_sensors` — Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git add include/core/SensorManager.h src/core/SensorManager.cpp test/test_sensors
git commit -m "feat(sensors): неблокирующий автомат опроса DS18B20 и AM2320"
```

## Task 2.2: Аппаратный адаптер датчиков и интеграция

**Files:**
- Create: `include/hal/ArduinoSensors.h`
- Create: `src/hal/ArduinoSensors.cpp`
- Modify: `src/main.cpp`

**Interfaces:**
- Produces: `class ArduinoSensors : public ISensorBus` с `begin()`, `requestFloorConversion()`, `readFloorTempC()`, `readAir(float&, float&)`, `readPir()`.

- [ ] **Step 1: Реализовать адаптер**

- `begin()`: `Wire.begin(PIN_SDA, PIN_SCL)`, `Wire.setClock(100000)`, `am2320.begin()`, `sensors.begin()`, `sensors.setWaitForConversion(false)`.
- `requestFloorConversion()`: `sensors.requestTemperatures()` (теперь возвращается сразу).
- `readFloorTempC()`: `sensors.getTempCByIndex(0)`.
- `readAir()`: **один** `am2320.readRegister32(0x00)` (сам выполняет пробуждение AM2320 — отдельный ACK-чек до пробуждения ненадёжен); декодировать температуру (знаковый бит 15) и влажность (`data >> 16`) как в библиотеке; вернуть `false` при `0xFFFFFFFF`.
- `readPir()`: `digitalRead(PIN_PIR) == HIGH`.

Остаточный риск: если после ACK чтение всё же уйдёт в повторы библиотеки, возможна пауза до ~300 мс. При появлении `null` в телеметрии — отдельная задача на свой неблокирующий драйвер AM2320 (вне этого плана).

- [ ] **Step 2: Подключить в `main.cpp`**

Создать `OneWire`, `DallasTemperature`, `Adafruit_AM2320`, `ArduinoSensors`, `SensorManager`; в `loop()` вызывать `sensors.update(now)`. Раз в 5 с печатать в Serial снимок (для проверки на железе). Никаких `delay()`.

- [ ] **Step 3: Собрать**

Run: `pio run` — Expected: SUCCESS.

- [ ] **Step 4: Commit**

```bash
git add include/hal/ArduinoSensors.h src/hal/ArduinoSensors.cpp src/main.cpp
git commit -m "feat(sensors): аппаратный адаптер и асинхронный опрос в loop"
```

## Block 2 gate — STOP

- [ ] `pio run`, `pio test -e native`; хвост вывода в отчёт.
- [ ] `graphify update .`; коммит графа.
- [ ] Отчёт: что проверено статически, что нужно проверить на железе (30-секундный опрос, отсутствие фризов).
- [ ] **Пауза** до команды «продолжай».

---

# Блок 3. Свет и CIE1931

## Task 3.1: Таблица CIE1931

**Files:**
- Create: `tools/gen_cie_lut.py`
- Create: `include/core/Cie1931.h`
- Create: `src/core/Cie1931.cpp`
- Create: `test/test_cie/test_cie.cpp`

**Interfaces:**
- Produces: `uint16_t cie1931(uint8_t percent);` — 0…1023.

- [ ] **Step 1: Написать падающий тест**

```cpp
TEST_ASSERT_EQUAL_UINT16(0, cie1931(0));
TEST_ASSERT_EQUAL_UINT16(1, cie1931(1));
TEST_ASSERT_EQUAL_UINT16(9, cie1931(8));
TEST_ASSERT_EQUAL_UINT16(10, cie1931(9));
TEST_ASSERT_EQUAL_UINT16(188, cie1931(50));
TEST_ASSERT_EQUAL_UINT16(1023, cie1931(100));
// монотонность: для p=1..100 cie1931(p) >= cie1931(p-1)
```

- [ ] **Step 2: Убедиться, что тест падает**

Run: `pio test -e native -f test_cie` — Expected: FAIL.

- [ ] **Step 3: Сгенерировать таблицу**

`tools/gen_cie_lut.py` печатает `Cie1931Table.h`:

```python
def cie(L):
    if L <= 0: return 0
    if L >= 100: return 1023
    out = L / 902.3 if L <= 8 else ((L + 16.0) / 116.0) ** 3
    return int(out * 1023.0)
```

Запуск: `python3 tools/gen_cie_lut.py > include/core/Cie1931Table.h`.

`Cie1931.h` объявляет `uint16_t cie1931(uint8_t percent);`. Сгенерированный `Cie1931Table.h` содержит `const uint16_t kCieLut[101] ARGON_PROGMEM = {...};`; `Cie1931.cpp` определяет `ARGON_PROGMEM` (`PROGMEM` на ESP, пусто на хосте) и `ARGON_READ_WORD` (`pgm_read_word` / разыменование) до включения таблицы. Таблица включается только из `Cie1931.cpp`, поэтому множественного определения нет.

- [ ] **Step 4: Прогнать тест**

Run: `pio test -e native -f test_cie` — Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git add tools/gen_cie_lut.py include/core/Cie1931.h include/core/Cie1931Table.h src/core/Cie1931.cpp test/test_cie
git commit -m "feat(light): таблица CIE1931 в PROGMEM вместо pow()"
```

## Task 3.2: `LightController`

**Files:**
- Create: `include/core/LightController.h`
- Create: `src/core/LightController.cpp`
- Create: `test/test_light/test_light.cpp`

**Interfaces:**
- Produces:
  - `class IPwmOutput { virtual void writeDuty(uint16_t duty) = 0; };`
  - `struct ILightObserver { virtual void onLightStateChanged(bool on) = 0; virtual void onBrightnessSettled(uint8_t percent) = 0; };`
  - `struct LightConfig { uint32_t fadeMs, stepMs, motionTimeoutMs, safetyTimeoutMs; uint8_t defaultBrightness; uint16_t pwmMax; };`
  - `class LightController { LightController(IPwmOutput&, ILightObserver&, LightConfig, uint32_t now); void setPower(bool, uint32_t); void setBrightness(uint8_t, uint32_t); void setMotionMode(bool, uint32_t); void onMotion(uint32_t); void update(uint32_t); bool isOn() const; bool motionMode() const; uint8_t brightness() const; uint32_t secondsSinceActivity(uint32_t now) const; };`

- [ ] **Step 1: Написать падающие тесты**

Обязательные кейсы:

```cpp
// 1. Старт: выключено, яркость 100, PWM 0.
// 2. setPower(true, 0): наблюдатель получил on=true сразу;
//    update(1000) -> duty 188 (50%); update(2000) -> 1023, settled(100).
// 3. setBrightness(30, 0) при включённом: на 1000 мс current=65,
//    duty=cie1931(65); на 2000 -> 30%, settled(30).
// 4. Реверс: на 1000 мс (50%) setPower(false, 1000); на 2000 -> 25%,
//    на 3000 -> 0%, settled(100).
// 5. HA: setBrightness(30) при выключенном -> setPower(true) даёт цель 30;
//    setPower(false) сбрасывает пользовательскую в 100; следующий ON -> 100.
// 6. Таймауты: motionMode=true -> off через 60000 мс; false -> 3600000 мс.
// 7. Любая команда света (power/brightness/motionMode) сбрасывает таймер.
// 8. Скачок now на 750 мс: яркость соответствует прошедшему времени, без перелёта.
// 9. Авто-выключение не сбрасывает secondsSinceActivity (в отличие от команды).
```

- [ ] **Step 2: Убедиться, что тесты падают**

Run: `pio test -e native -f test_light` — Expected: FAIL.

- [ ] **Step 3: Реализовать контроллер**

Правила:
- `setPower(false)` сначала публикует `onLightStateChanged(false)`, затем `userBrightness_ = 100`, цель 0. `setPower(true)` — цель `userBrightness_`.
- `setBrightness` зажимает 1…100, пишет `userBrightness_`, сбрасывает таймер; если включено — задаёт цель.
- `setMotionMode` только меняет режим и сбрасывает таймер.
- `onMotion` сбрасывает таймер; если режим движения и свет выключен — `setPower(true)`.
- `update`: сначала авто-выключение по таймауту (60 с / 3600 с), затем фейдер.
- Авто-выключение вызывает внутренний `applyPower(false)`: он сбрасывает `userBrightness_` в 100, но **не** трогает `lastActivity` — `since_move_s` продолжает расти, как в легаси. Публичные команды (`setPower`, `setBrightness`, `setMotionMode`, `onMotion`) сбрасывают таймер.
- Фейдер: `progress = min(1, (now - fadeStart)/fadeMs)`, `current = start + (target-start)*progress`; PWM обновляется не чаще `stepMs`; по завершении — `onBrightnessSettled(userBrightness_)`.
- `secondsSinceActivity(now) = (now - lastActivity)/1000`.
- Начальное состояние: выключено, цель 0, PWM 0, `lastActivity = now`.

- [ ] **Step 4: Прогнать тесты**

Run: `pio test -e native -f test_light` — Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git add include/core/LightController.h src/core/LightController.cpp test/test_light
git commit -m "feat(light): контроллер света с времязависимым фейдером"
```

## Task 3.3: PWM-адаптер и интеграция света

**Files:**
- Create: `include/hal/ArduinoPwm.h`
- Create: `src/hal/ArduinoPwm.cpp`
- Modify: `src/main.cpp`

**Interfaces:**
- Produces: `class ArduinoPwm : public IPwmOutput` с `begin()` и `writeDuty(uint16_t)`.

- [ ] **Step 1: Реализовать адаптер**

`begin()`: `analogWriteFreq(cfg::PWM_FREQ_HZ)`, `analogWriteRange(cfg::PWM_RANGE)`, `pinMode(PIN_PWM_LED, OUTPUT)`, `analogWrite(PIN_PWM_LED, 0)`. `writeDuty(d)`: `analogWrite(PIN_PWM_LED, constrain(d, 0, cfg::PWM_RANGE))`.

- [ ] **Step 2: Подключить `LightController` в `main.cpp`**

Создать `ArduinoPwm`, наблюдателя с логами `[LIGHT]`, `LightController`; в `loop()` вызывать `light.update(now)`. Пока без MQTT: команды не приходят, но скелет компилируется и стартует с выключенным светом.

- [ ] **Step 3: Собрать и прогнать все тесты**

Run: `pio run` — Expected: SUCCESS.
Run: `pio test -e native` — Expected: PASS.

- [ ] **Step 4: Commit**

```bash
git add include/hal/ArduinoPwm.h src/hal/ArduinoPwm.cpp src/main.cpp
git commit -m "feat(light): PWM-адаптер и подключение контроллера света"
```

## Block 3 gate — STOP

- [ ] `pio run`, `pio test -e native`; хвост вывода.
- [ ] `graphify update .`; коммит графа.
- [ ] Отчёт: проверить на железе плавность 2 с, реверс, ловушку HA (вручную командами пока нельзя — только после блока 5).
- [ ] **Пауза**.

---

# Блок 4. Тёплый пол

## Task 4.1: `FloorThermostat`

**Files:**
- Create: `include/core/FloorThermostat.h`
- Create: `src/core/FloorThermostat.cpp`
- Create: `test/test_floor/test_floor.cpp`

**Interfaces:**
- Produces:
  - `class IRelayOutput { virtual void writeRelay(bool on) = 0; };`
  - `struct IFloorObserver { virtual void onMasterChanged(bool on) = 0; virtual void onHeatChanged(bool on) = 0; };`
  - `struct FloorConfig { float hysteresisC; uint32_t motionTimeoutMs, safetyTimeoutMs, staleTimeoutMs; float defaultTargetC; };`
  - `class FloorThermostat { FloorThermostat(IRelayOutput&, IFloorObserver&, FloorConfig, uint32_t now); void setMaster(bool, uint32_t); void setMotionMode(bool, uint32_t); void setTarget(float, uint32_t); void onMotion(uint32_t); void onSample(float tempC, uint32_t now); void update(uint32_t now); bool master() const; bool heat() const; bool motionMode() const; float target() const; };`

- [ ] **Step 1: Написать падающие тесты**

```cpp
// 1. Старт: master=false, heat=false, target=30.
// 2. Гистерезис: target 30, sample 29.4 -> heat=true;
//    29.6 -> heat остаётся true; 30.6 -> heat=false; 30.4 -> остаётся false.
// 3. Смена уставки: sample 30.0 при target 30 -> heat не меняется (зона гистерезиса);
//    setTarget(31.0) -> 30.0 < 30.5 -> heat=true.
// 4. PIR при motionMode=true -> master=true, onMasterChanged(true).
// 5. Таймаут master: motionMode=true -> 3600000 мс; false -> 43200000 мс;
//    по истечении master=false, onMasterChanged(false), heat=false.
// 6. Команда floor (master/motion/target) сбрасывает таймер.
// 7. Fail-safe: после валидного sample 90 000 мс без нового -> heat=false,
//    master остаётся true; после нового sample нагрев возобновляется.
// 8. onHeatChanged вызывается только при реальной смене состояния реле.
```

- [ ] **Step 2: Убедиться, что тесты падают**

Run: `pio test -e native -f test_floor` — Expected: FAIL.

- [ ] **Step 3: Реализовать термостат**

Правила:
- `lastActivity` сбрасывается `setMaster`, `setMotionMode`, `setTarget`, `onMotion`.
- `onMotion`: если `motionMode && !master` — `master = true`, `onMasterChanged(true)`.
- `update`: если `master && (now - lastActivity) >= timeout` — `master=false`, `onMasterChanged(false)`.
- Окно активно, если `(now - lastActivity) < timeout`.
- Нагрев возможен только при `master && windowActive && hasValidSample && (now - lastValidSample) < staleTimeout`.
- Гистерезис: `temp < target - h` → heat=true; `temp > target + h` → heat=false; иначе не менять.
- При выключении мастера/неактивном окне/устаревших данных — `heat=false`.
- `onHeatChanged` вызывать только при изменении; `writeRelay` — тоже только при изменении.
- `onSample` проверяет конечность и диапазон (защита от дурака), обновляет `lastValidSample`.

- [ ] **Step 4: Прогнать тесты**

Run: `pio test -e native -f test_floor` — Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git add include/core/FloorThermostat.h src/core/FloorThermostat.cpp test/test_floor
git commit -m "feat(floor): термостат с гистерезисом, таймерами и fail-safe"
```

## Task 4.2: Реле и интеграция пола

**Files:**
- Create: `include/hal/ArduinoRelay.h`
- Create: `src/hal/ArduinoRelay.cpp`
- Modify: `src/main.cpp`

**Interfaces:**
- Produces: `class ArduinoRelay : public IRelayOutput` с `begin()` и `writeRelay(bool)`.

- [ ] **Step 1: Реализовать адаптер**

`begin()`: `pinMode(PIN_SSR_FLOOR, OUTPUT)`, `digitalWrite(PIN_SSR_FLOOR, LOW)`. `writeRelay(on)`: `digitalWrite(PIN_SSR_FLOOR, on ? HIGH : LOW)`.

- [ ] **Step 2: Подключить в `main.cpp`**

- Создать `ArduinoRelay`, наблюдателя `[FLOOR]`, `FloorThermostat`.
- В `loop()` после `sensors.update(now)`: если `snapshot.floorSampleMs` изменился — `floor.onSample(snapshot.floorTempC, now)`.
- Если `sensors.takeMotionStarted()` — `light.onMotion(now)`, `floor.onMotion(now)`; если `takeMotionStopped()` — пока только лог.
- Вызывать `floor.update(now)`.

- [ ] **Step 3: Собрать и прогнать тесты**

Run: `pio run` — Expected: SUCCESS.
Run: `pio test -e native` — Expected: PASS.

- [ ] **Step 4: Commit**

```bash
git add include/hal/ArduinoRelay.h src/hal/ArduinoRelay.cpp src/main.cpp
git commit -m "feat(floor): реле SSR и интеграция термостата"
```

## Block 4 gate — STOP

- [ ] `pio run`, `pio test -e native`; хвост вывода.
- [ ] `graphify update .`; коммит графа.
- [ ] Отчёт: проверено статически/тестами; на железе — гистерезис и fail-safe после блока 5.
- [ ] **Пауза**.

---

# Блок 5. Сеть, MQTT и телеметрия

## Task 5.1: Формирование JSON

**Files:**
- Create: `include/core/TelemetryFormatter.h`
- Create: `src/core/TelemetryFormatter.cpp`
- Create: `test/test_telemetry/test_telemetry.cpp`

**Interfaces:**
- Produces:
  - `struct TelemetryData { bool lightOn, lightMotionMode, floorMaster, floorMotionMode, floorHeat; bool floorValid; float floorTempC; bool airValid; float airTempC, airHumPct; bool pirRaw; uint32_t sinceMoveS, uptimeS, heapBytes; };`
  - `size_t formatTelemetry(const TelemetryData&, char* out, size_t outSize);`

- [ ] **Step 1: Написать падающие тесты**

```cpp
TelemetryData d{};
d.lightOn = true; d.lightMotionMode = true;
d.floorMaster = true; d.floorMotionMode = true; d.floorHeat = false;
d.floorValid = true; d.floorTempC = 27.44f;
d.airValid = true; d.airTempC = 22.14f; d.airHumPct = 41.54f;
d.pirRaw = false; d.sinceMoveS = 37; d.uptimeS = 86400; d.heapBytes = 31240;

char buf[512];
formatTelemetry(d, buf, sizeof(buf));
TEST_ASSERT_EQUAL_STRING(
  "{\"light_state\":\"ON\",\"light_move_state\":\"ON\",\"floor_state\":\"ON\","
  "\"floor_move_state\":\"ON\",\"floor_heat_state\":\"OFF\",\"floor_temp\":27.4,"
  "\"air_temp\":22.1,\"air_hum\":41.5,\"pir_raw\":false,\"since_move_s\":37,"
  "\"uptime\":86400,\"heap\":31240}", buf);

// невалидные датчики -> floor_temp/air_temp/air_hum = null
// длина результата < 512
```

- [ ] **Step 2: Убедиться, что тест падает**

Run: `pio test -e native -f test_telemetry` — Expected: FAIL.

- [ ] **Step 3: Реализовать форматтер**

`StaticJsonDocument<512>`, поля в строгом порядке из Global Constraints; температуры — `(float)((int)(t * 10)) / 10.0f` (как в легаси); невалидные — `nullptr`; `serializeJson(doc, out, outSize)`. Вернуть длину.

- [ ] **Step 4: Прогнать тест**

Run: `pio test -e native -f test_telemetry` — Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git add include/core/TelemetryFormatter.h src/core/TelemetryFormatter.cpp test/test_telemetry
git commit -m "feat(telemetry): JSON с фиксированным порядком ключей"
```

## Task 5.2: `NetworkManager` и MQTT

**Files:**
- Create: `include/NetworkManager.h`
- Create: `src/NetworkManager.cpp`
- Modify: `src/main.cpp`

**Interfaces:**
- Produces:
  - `struct IMqttCommandSink { virtual void onLightPower(bool) = 0; virtual void onLightBrightness(uint8_t) = 0; virtual void onLightMotionMode(bool) = 0; virtual void onFloorPower(bool) = 0; virtual void onFloorMotionMode(bool) = 0; virtual void onFloorTarget(float) = 0; };`
  - `class NetworkManager { NetworkManager(IMqttCommandSink&, const char* ssid, const char* pass, const char* user, const char* pass, const char* clientId); void begin(); void update(uint32_t now); bool connected() const; bool publish(const char* topic, const char* payload, bool retained); };`

- [ ] **Step 0: Локальные секреты**

Скопировать `include/Secrets.h.example` в `include/Secrets.h` и заполнить значениями (файл в `.gitignore`, не коммитить). Для проверки сборки допустимы заглушки.

- [ ] **Step 1: Реализовать менеджер**

- `begin()`: `WiFi.mode(WIFI_STA)`, `WiFi.begin(ssid, pass)`, `client.setCallback`, `client.setBufferSize(512)`, `espClient.setTimeout(2000)` (верхняя граница TCP-connect, чтобы обрыв брокера не блокировал `loop()` дольше 2 с); без ожидания подключения.
- `update(now)`: `client.loop()`; если Wi-Fi подключён и MQTT нет — попытка не чаще `MQTT_RETRY_MS`, `setServer(WiFi.gatewayIP(), 1883)`, `connect(clientId, user, pass)`; при успехе — первичная публикация состояний и подписки.
- Если связи нет дольше `MQTT_LOST_REBOOT_MS` — `ESP.restart()`.
- Первичная публикация: все `*_state` = OFF, `brightness_state` = 100, `targetTemp` = 30.0, `light/move` = OFF (не retain).
- Парсинг команд: `ON`/`OFF`, brightness 1…100, targetTemp float; вызовы `IMqttCommandSink`.

- [ ] **Step 2: Подключить в `main.cpp`**

- `CommandSink` вызывает `light.setPower`, `light.setBrightness`, `light.setMotionMode`, `floor.setMaster`, `floor.setMotionMode`, `floor.setTarget`; передаёт `millis()`.
- Порядок создания: `NetworkManager` → контроллеры с наблюдателями, ссылающимися на `network` → `sink.attach(&light, &floor)` → `network.begin()`. До `begin()` наблюдатели ничего не публикуют.
- Наблюдатели света/пола публикуют `light_state`, `brightness_state` (по завершении), `floor_state`, `floor_heat_state`.
- PIR-фронты публикуют `light/move` ON/OFF (без retain); `takeMotionStarted`/`takeMotionStopped`.
- Телеметрия: раз в 10 с собрать `TelemetryData` из снимка и контроллеров, `formatTelemetry`, `client.publish(TOPIC_STATE, buf, false)`.
- `since_move_s` — `light.secondsSinceActivity(now)`; `heap` — `ESP.getFreeHeap()`.
- Обработчик `floor_state` по таймауту уже вызывает наблюдатель — публикация произойдёт.

- [ ] **Step 3: Собрать**

Run: `pio run` — Expected: SUCCESS.

- [ ] **Step 4: Commit**

```bash
git add include/NetworkManager.h src/NetworkManager.cpp src/main.cpp
git commit -m "feat(net): Wi-Fi/MQTT, команды и телеметрия"
```

## Task 5.3: Сквозная проверка и документация

**Files:**
- Modify: `README.md` (тесты, статус)
- Modify: `AGENTS.md` (текущее состояние)
- Maybe: `docs/freeze-analysis.md` (результаты замеров)

- [ ] **Step 1: Прогнать всё**

```bash
pio test -e native
pio run
```

Expected: PASS/SUCCESS. Приложить хвосты.

- [ ] **Step 2: Ручной чек-лист на железе**

- [ ] Старт: свет/пол выключены, в MQTT ушли OFF и targetTemp 30.0.
- [ ] PIR первые 10 с игнорируется.
- [ ] Розжиг 2 с, затухание 2 с, реверс с текущей точки.
- [ ] `brightness` 30 → `ON`: включается на 30; `OFF` → `ON`: 100.
- [ ] Свет: таймаут 1 мин / 1 ч; любая команда света сбрасывает таймер.
- [ ] Пол: гистерезис ±0.5, автовключение по PIR, таймауты 1 ч / 12 ч.
- [ ] Телеметрия: порядок ключей, `null` при ошибках, интервал 10 с.
- [ ] Обрыв MQTT: реконнект раз в 5 с; ребут через 5 мин без связи.
- [ ] Нет пауз в анимации на тиках датчиков (AM2320 15 с, DS18B20 30 с); замерить `[PERF]` логи.

- [ ] **Step 3: Обновить README/AGENTS**

README: добавить `pio test -e native`, статус «модульная прошивка собрана», актуальные ограничения. AGENTS §3: `src/` заполнен, тесты есть; убрать «появится в Блоке 1».

- [ ] **Step 4: Граф и коммит**

```bash
graphify update .
git add -A
git commit -m "feat: завершить рефакторинг Argon"
```

## Block 5 gate — STOP

- [ ] `pio run`, `pio test -e native`, график, коммит.
- [ ] Финальный отчёт: что проверено на железе, что осталось (например, длительный прогон).
- [ ] **Пауза**; дальнейшие изменения — только по запросу.

---

## Примечания по реализации

- `Config.h` и `MqttTopics.h` не должны включать Arduino — их использует native-сборка.
- Чистые модули (`src/core/`) не включают Arduino; время передаётся параметром `now`, а не читается внутри.
- Для тестов используйте маленькие интервалы через конфиги (`LightConfig`, `FloorConfig`, `SensorConfig`), не меняя боевые значения.
- `pio test -e native` не должен компилировать `src/main.cpp` и `src/hal/` — за это отвечает `build_src_filter = +<core/>` и `test_build_src = yes`.
- Если ArduinoJson на хосте сериализует float иначе, чем ожидается, проверьте фактическую строку и зафиксируйте её в тесте; формат чисел — часть контракта.
- Не коммитить `include/Secrets.h`, `.pio/`, `graphify-out/cache/`, `graphify-out/graph.html`.
