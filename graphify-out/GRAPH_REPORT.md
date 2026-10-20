# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 39 files · ~18,136 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 470 nodes · 754 edges · 24 communities (19 shown, 5 thin omitted)
- Extraction: 77% EXTRACTED · 23% INFERRED · 0% AMBIGUOUS · INFERRED: 170 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `efaaa6e7`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- AGENTS.md
- main.cpp
- Smart Home Hall — умный коридор на ESP8266
- SensorManager
- LightController
- FloorThermostat
- NetworkManager
- CommandSink
- LightController.cpp
- test_sensors.cpp
- FakeBus
- NetworkManager.cpp
- 3. Текущее состояние
- NetworkManager.cpp
- FloorThermostat.cpp
- SensorSnapshot
- IMqttCommandSink
- ISensorBus
- SerialFloorObserver
- SerialLightObserver
- План реализации рефакторинга Argon
- FloorState
- SensorManager.h

## God Nodes (most connected - your core abstractions)
1. `LightController` - 37 edges
2. `NetworkManager` - 36 edges
3. `FloorThermostat` - 32 edges
4. `SensorManager` - 26 edges
5. `ArduinoSensors` - 15 edges
6. `CommandSink` - 14 edges
7. `IMqttCommandSink` - 13 edges
8. `FakeBus` - 13 edges
9. `SensorSnapshot` - 12 edges
10. `heat` - 12 edges

## Surprising Connections (you probably didn't know these)
- `6. Целевое решение (эскиз, не финальный план)` --references--> `FloorThermostat`  [INFERRED]
  docs/freeze-analysis.md → include/core/FloorThermostat.h
- `Task 5.2: `NetworkManager` и MQTT` --references--> `IMqttCommandSink`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/NetworkManager.h
- `План реализации рефакторинга Argon` --references--> `IMqttCommandSink`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/NetworkManager.h
- `5. Архитектура` --references--> `NetworkManager`  [INFERRED]
  AGENTS.md → include/NetworkManager.h
- `Task 5.2: `NetworkManager` и MQTT` --references--> `NetworkManager`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/NetworkManager.h

## Import Cycles
- None detected.

## Communities (24 total, 5 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.08
Nodes (23): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+15 more)

### Community 1 - "main.cpp"
Cohesion: 0.08
Nodes (22): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию (+14 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.06
Nodes (25): Claude Code, Правила, С чего начать после `git clone`, Текущий статус, Замечания, Испытания на железе, После испытаний, Проверено (+17 more)

### Community 3 - "SensorManager"
Cohesion: 0.13
Nodes (16): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Блок 2. Датчики и неблокирующий опрос, SensorManager, airInitialized_, bootMs_, config_ (+8 more)

### Community 4 - "LightController"
Cohesion: 0.10
Nodes (28): Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.2: `LightController`, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, LightController, config_, current_ (+20 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (26): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+18 more)

### Community 6 - "NetworkManager"
Cohesion: 0.09
Nodes (16): NetworkManager, attempted_, client_, clientId_, initialPublished_, instance_, lastAttemptMs_, lastTargetC_ (+8 more)

### Community 7 - "CommandSink"
Cohesion: 0.18
Nodes (4): CommandSink, floor_, light_, setup()

### Community 8 - "LightController.cpp"
Cohesion: 0.05
Nodes (25): ILightObserver, IPwmOutput, LightConfig, defaultBrightness, fadeMs, motionTimeoutMs, pwmMax, safetyTimeoutMs (+17 more)

### Community 9 - "test_sensors.cpp"
Cohesion: 0.24
Nodes (10): Task 5.2: `NetworkManager` и MQTT, snapshot_, test_air_failure_invalidates_and_recovers(), test_first_update_requests_floor_and_reads_air(), test_floor_reads_after_conversion_time(), test_invalid_floor_reading_invalidates_sample(), test_invalid_floor_values_are_rejected(), test_pir_high_at_boot_does_not_trigger_after_warmup() (+2 more)

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 11 - "NetworkManager.cpp"
Cohesion: 0.06
Nodes (6): formatTelemetry(), truncToTenths(), loop(), publishTelemetry(), test_full_json_has_fixed_key_order(), test_invalid_sensors_are_null()

### Community 12 - "3. Текущее состояние"
Cohesion: 0.10
Nodes (16): 3. Текущее состояние, Block 1 gate — STOP, Block 4 gate — STOP, Block 5 gate — STOP, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки, Task 4.2: Реле и интеграция пола, Task 5.1: Формирование JSON (+8 more)

### Community 13 - "NetworkManager.cpp"
Cohesion: 0.15
Nodes (3): NetworkManager::callbackTrampoline(), NetworkManager::handleMessage(), NetworkManager::update()

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.08
Nodes (20): FloorConfig, defaultTargetC, hysteresisC, motionTimeoutMs, safetyTimeoutMs, staleTimeoutMs, IFloorObserver, IRelayOutput (+12 more)

### Community 15 - "SensorSnapshot"
Cohesion: 0.20
Nodes (10): SensorSnapshot, airHumPct, airTempC, airValid, floorLastReadC, floorSampleMs, floorTempC, floorValid (+2 more)

### Community 20 - "План реализации рефакторинга Argon"
Cohesion: 0.50
Nodes (4): File Structure, Global Constraints, Review Focus, План реализации рефакторинга Argon

### Community 21 - "FloorState"
Cohesion: 0.67
Nodes (3): FloorState, Idle, Waiting

### Community 24 - "SensorManager.h"
Cohesion: 0.20
Nodes (6): SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, SensorManager::SensorManager()

## Knowledge Gaps
- **91 isolated node(s):** `ssid_`, `wifiPassword_`, `mqttUser_`, `mqttPassword_`, `clientId_` (+86 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 246 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **5 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `CommandSink`, `3. Текущее состояние`, `FloorThermostat.cpp`?**
  _High betweenness centrality (0.204) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `ssid_`, `wifiPassword_`, `mqttUser_` to the rest of the system?**
  _91 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.08333333333333333 - nodes in this community are weakly interconnected._
- **Why does `LightController` connect `LightController` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `CommandSink`, `LightController.cpp`, `3. Текущее состояние`?**
  _High betweenness centrality (0.203) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `NetworkManager` (e.g. with `5. Архитектура` and `Task 5.2: `NetworkManager` и MQTT`) actually correct?**
  _`NetworkManager` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07936507936507936 - nodes in this community are weakly interconnected._