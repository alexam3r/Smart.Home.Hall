# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 27 files · ~14,407 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 301 nodes · 448 edges · 14 communities (13 shown, 1 thin omitted)
- Extraction: 77% EXTRACTED · 23% INFERRED · 0% AMBIGUOUS · INFERRED: 101 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `d84d9069`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- AGENTS.md
- main.cpp
- Smart Home Hall — умный коридор на ESP8266
- ISensorBus
- LightController
- SensorManager
- 2026-10-20-argon-refactoring.md
- test_sensors.cpp
- LightController.cpp
- ArduinoPwm
- FakeBus
- SensorSnapshot
- ILightObserver

## God Nodes (most connected - your core abstractions)
1. `LightController` - 36 edges
2. `SensorManager` - 26 edges
3. `ArduinoSensors` - 15 edges
4. `FakeBus` - 13 edges
5. `ISensorBus` - 11 edges
6. `LightConfig` - 10 edges
7. `SensorSnapshot` - 10 edges
8. `Smart Home Hall — умный коридор на ESP8266` - 10 edges
9. `Зависания анимации на 1–2 с: анализ причин` - 10 edges
10. `IPwmOutput` - 8 edges

## Surprising Connections (you probably didn't know these)
- `9. Рабочий процесс (обязательный)` --references--> `LightController`  [INFERRED]
  AGENTS.md → include/core/LightController.h
- `3. Текущее состояние` --references--> `LightController`  [INFERRED]
  AGENTS.md → include/core/LightController.h
- `5. Целевая архитектура` --references--> `LightController`  [INFERRED]
  AGENTS.md → include/core/LightController.h
- `Task 3.3: PWM-адаптер и интеграция света` --references--> `LightController`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/core/LightController.h
- `3.1. Модульность и Безопасность` --references--> `LightController`  [INFERRED]
  PRD.md → include/core/LightController.h

## Import Cycles
- None detected.

## Communities (14 total, 1 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.07
Nodes (24): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+16 more)

### Community 1 - "main.cpp"
Cohesion: 0.07
Nodes (22): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию (+14 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.14
Nodes (14): MQTT API, Smart Home Hall — умный коридор на ESP8266, Возможности, Документация, Железо, Команды (контроллер подписан, без retain), Рефакторинг, Сборка и прошивка (+6 more)

### Community 3 - "ISensorBus"
Cohesion: 0.12
Nodes (9): ISensorBus, SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, isValidFloorTemp(), SensorManager::SensorManager() (+1 more)

### Community 4 - "LightController"
Cohesion: 0.11
Nodes (24): Task 3.2: `LightController`, Task 4.1: `FloorThermostat`, LightController, config_, current_, fadeStartMs_, fading_, lastActivityMs_ (+16 more)

### Community 5 - "SensorManager"
Cohesion: 0.11
Nodes (19): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle, Waiting, SensorManager (+11 more)

### Community 6 - "2026-10-20-argon-refactoring.md"
Cohesion: 0.07
Nodes (27): Block 1 gate — STOP, Block 4 gate — STOP, Block 5 gate — STOP, File Structure, Global Constraints, Review Focus, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки (+19 more)

### Community 7 - "test_sensors.cpp"
Cohesion: 0.22
Nodes (11): Task 4.2: Реле и интеграция пола, Task 5.2: `NetworkManager` и MQTT, snapshot_, test_air_failure_invalidates_and_recovers(), test_first_update_requests_floor_and_reads_air(), test_floor_reads_after_conversion_time(), test_invalid_floor_reading_keeps_last_valid(), test_invalid_floor_values_are_rejected() (+3 more)

### Community 8 - "LightController.cpp"
Cohesion: 0.08
Nodes (6): cie1931(), LightController::applyAutoOff(), LightController::dutyFor(), LightController::onMotion(), LightController::setBrightness(), LightController::setPower()

### Community 9 - "ArduinoPwm"
Cohesion: 0.18
Nodes (8): 3. Текущее состояние, Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, ArduinoPwm, cie(), main()

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 11 - "SensorSnapshot"
Cohesion: 0.22
Nodes (9): SensorSnapshot, airHumPct, airTempC, airValid, floorSampleMs, floorTempC, floorValid, lastMotionMs (+1 more)

### Community 12 - "ILightObserver"
Cohesion: 0.14
Nodes (7): ILightObserver, SerialLightObserver, FakeObserver, on, settledCount, settledPercent, stateChanges

## Knowledge Gaps
- **57 isolated node(s):** `fadeMs`, `stepMs`, `motionTimeoutMs`, `safetyTimeoutMs`, `defaultBrightness` (+52 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 163 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `LightController` connect `LightController` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `2026-10-20-argon-refactoring.md`, `LightController.cpp`, `ArduinoPwm`, `ILightObserver`?**
  _High betweenness centrality (0.326) - this node is a cross-community bridge._
- **Are the 8 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Целевая архитектура`) actually correct?**
  _`LightController` has 8 INFERRED edges - model-reasoned connections that need verification._
- **What connects `fadeMs`, `stepMs`, `motionTimeoutMs` to the rest of the system?**
  _57 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.06653225806451613 - nodes in this community are weakly interconnected._
- **Why does `SensorManager` connect `SensorManager` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `ISensorBus`, `test_sensors.cpp`, `ArduinoPwm`, `SensorSnapshot`?**
  _High betweenness centrality (0.246) - this node is a cross-community bridge._
- **Are the 6 inferred relationships involving `SensorManager` (e.g. with `3. Текущее состояние` and `5. Целевая архитектура`) actually correct?**
  _`SensorManager` has 6 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07396870554765292 - nodes in this community are weakly interconnected._