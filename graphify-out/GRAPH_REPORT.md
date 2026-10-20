# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 32 files · ~15,338 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 379 nodes · 608 edges · 15 communities (13 shown, 2 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 148 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `2a45c48d`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- AGENTS.md
- main.cpp
- Smart Home Hall — умный коридор на ESP8266
- SensorManager
- LightController
- FloorThermostat
- LightConfig
- test_sensors.cpp
- LightController.cpp
- IRelayOutput
- FakeBus
- src/main.cpp
- ILightObserver
- FloorThermostat.cpp

## God Nodes (most connected - your core abstractions)
1. `LightController` - 35 edges
2. `FloorThermostat` - 30 edges
3. `SensorManager` - 26 edges
4. `ArduinoSensors` - 15 edges
5. `FakeBus` - 13 edges
6. `ISensorBus` - 11 edges
7. `LightConfig` - 10 edges
8. `SensorSnapshot` - 10 edges
9. `heat` - 10 edges
10. `Smart Home Hall — умный коридор на ESP8266` - 10 edges

## Surprising Connections (you probably didn't know these)
- `6. Целевое решение (эскиз, не финальный план)` --references--> `FloorThermostat`  [INFERRED]
  docs/freeze-analysis.md → include/core/FloorThermostat.h
- `3. Текущее состояние` --references--> `FloorThermostat`  [INFERRED]
  AGENTS.md → include/core/FloorThermostat.h
- `5. Целевая архитектура` --references--> `FloorThermostat`  [INFERRED]
  AGENTS.md → include/core/FloorThermostat.h
- `9. Рабочий процесс (обязательный)` --references--> `FloorThermostat`  [INFERRED]
  AGENTS.md → include/core/FloorThermostat.h
- `Task 4.2: Реле и интеграция пола` --references--> `FloorThermostat`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/core/FloorThermostat.h

## Import Cycles
- None detected.

## Communities (15 total, 2 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.07
Nodes (24): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+16 more)

### Community 1 - "main.cpp"
Cohesion: 0.07
Nodes (22): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию (+14 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.12
Nodes (15): ArduinoRelay, MQTT API, Smart Home Hall — умный коридор на ESP8266, Возможности, Документация, Железо, Команды (контроллер подписан, без retain), Рефакторинг (+7 more)

### Community 3 - "SensorManager"
Cohesion: 0.05
Nodes (37): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle, Waiting, ISensorBus (+29 more)

### Community 4 - "LightController"
Cohesion: 0.11
Nodes (23): Task 3.2: `LightController`, LightController, config_, current_, fadeStartMs_, fading_, lastActivityMs_, lastStepMs_ (+15 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (24): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+16 more)

### Community 6 - "LightConfig"
Cohesion: 0.11
Nodes (18): Block 5 gate — STOP, Task 5.1: Формирование JSON, Task 5.3: Сквозная проверка и документация, Блок 5. Сеть, MQTT и телеметрия, Примечания по реализации, FloorConfig, defaultTargetC, hysteresisC (+10 more)

### Community 7 - "test_sensors.cpp"
Cohesion: 0.24
Nodes (10): Task 5.2: `NetworkManager` и MQTT, snapshot_, test_air_failure_invalidates_and_recovers(), test_first_update_requests_floor_and_reads_air(), test_floor_reads_after_conversion_time(), test_invalid_floor_reading_keeps_last_valid(), test_invalid_floor_values_are_rejected(), test_pir_high_at_boot_does_not_trigger_after_warmup() (+2 more)

### Community 8 - "LightController.cpp"
Cohesion: 0.07
Nodes (15): 3. Текущее состояние, IPwmOutput, ArduinoPwm, cie1931(), LightController::applyAutoOff(), LightController::dutyFor(), LightController::LightController(), LightController::onMotion() (+7 more)

### Community 9 - "IRelayOutput"
Cohesion: 0.08
Nodes (20): Block 1 gate — STOP, Block 3 gate — STOP, Block 4 gate — STOP, File Structure, Global Constraints, Review Focus, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки (+12 more)

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 12 - "ILightObserver"
Cohesion: 0.14
Nodes (7): ILightObserver, SerialLightObserver, FakeObserver, on, settledCount, settledPercent, stateChanges

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.27
Nodes (8): FloorThermostat::onMotion(), FloorThermostat::onSample(), FloorThermostat::recomputeHeat(), FloorThermostat::setMaster(), FloorThermostat::setMotionMode(), FloorThermostat::setTarget(), FloorThermostat::update(), isValidFloorTemp()

## Knowledge Gaps
- **71 isolated node(s):** `hysteresisC`, `motionTimeoutMs`, `safetyTimeoutMs`, `staleTimeoutMs`, `defaultTargetC` (+66 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 195 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `LightConfig`, `LightController.cpp`, `IRelayOutput`, `src/main.cpp`, `FloorThermostat.cpp`?**
  _High betweenness centrality (0.250) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Целевая архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `hysteresisC`, `motionTimeoutMs`, `safetyTimeoutMs` to the rest of the system?**
  _71 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.06653225806451613 - nodes in this community are weakly interconnected._
- **Why does `LightController` connect `LightController` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `LightConfig`, `LightController.cpp`, `IRelayOutput`, `ILightObserver`?**
  _High betweenness centrality (0.238) - this node is a cross-community bridge._
- **Are the 9 inferred relationships involving `FloorThermostat` (e.g. with `3. Текущее состояние` and `5. Целевая архитектура`) actually correct?**
  _`FloorThermostat` has 9 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07396870554765292 - nodes in this community are weakly interconnected._