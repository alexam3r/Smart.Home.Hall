# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 37 files · ~16,930 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 457 nodes · 728 edges · 25 communities (17 shown, 8 thin omitted)
- Extraction: 78% EXTRACTED · 22% INFERRED · 0% AMBIGUOUS · INFERRED: 160 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `28ee58d6`
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
- NetworkManager.cpp
- LightController.cpp
- План реализации рефакторинга Argon
- FakeBus
- src/main.cpp
- SerialLightObserver
- FloorThermostat.cpp
- formatTelemetry
- CommandSink
- SensorSnapshot
- IMqttCommandSink
- 3. Текущее состояние
- SerialFloorObserver
- SensorConfig

## God Nodes (most connected - your core abstractions)
1. `LightController` - 37 edges
2. `NetworkManager` - 35 edges
3. `FloorThermostat` - 31 edges
4. `SensorManager` - 26 edges
5. `ArduinoSensors` - 15 edges
6. `CommandSink` - 14 edges
7. `IMqttCommandSink` - 13 edges
8. `FakeBus` - 13 edges
9. `ISensorBus` - 11 edges
10. `SensorSnapshot` - 11 edges

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

## Communities (25 total, 8 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.06
Nodes (27): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+19 more)

### Community 1 - "main.cpp"
Cohesion: 0.07
Nodes (22): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию (+14 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.12
Nodes (16): MQTT API, Smart Home Hall — умный коридор на ESP8266, Архитектура, Возможности, Для AI-агентов, Документация, Железо, Команды (контроллер подписан, без retain) (+8 more)

### Community 3 - "SensorManager"
Cohesion: 0.08
Nodes (30): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Task 4.2: Реле и интеграция пола, Task 5.2: `NetworkManager` и MQTT, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle (+22 more)

### Community 4 - "LightController"
Cohesion: 0.09
Nodes (27): Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.2: `LightController`, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, LightController, config_, current_ (+19 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (24): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+16 more)

### Community 6 - "NetworkManager"
Cohesion: 0.10
Nodes (15): NetworkManager, attempted_, client_, clientId_, initialPublished_, instance_, lastAttemptMs_, lastWifiAttemptMs_ (+7 more)

### Community 7 - "NetworkManager.cpp"
Cohesion: 0.17
Nodes (3): NetworkManager::callbackTrampoline(), NetworkManager::handleMessage(), NetworkManager::update()

### Community 8 - "LightController.cpp"
Cohesion: 0.05
Nodes (24): ILightObserver, IPwmOutput, LightConfig, defaultBrightness, fadeMs, motionTimeoutMs, pwmMax, safetyTimeoutMs (+16 more)

### Community 9 - "План реализации рефакторинга Argon"
Cohesion: 0.50
Nodes (4): File Structure, Global Constraints, Review Focus, План реализации рефакторинга Argon

### Community 10 - "FakeBus"
Cohesion: 0.11
Nodes (9): ISensorBus, FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp (+1 more)

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.08
Nodes (21): FloorConfig, defaultTargetC, hysteresisC, motionTimeoutMs, safetyTimeoutMs, staleTimeoutMs, IFloorObserver, IRelayOutput (+13 more)

### Community 15 - "formatTelemetry"
Cohesion: 0.23
Nodes (4): formatTelemetry(), truncToTenths(), test_full_json_has_fixed_key_order(), test_invalid_sensors_are_null()

### Community 16 - "CommandSink"
Cohesion: 0.18
Nodes (4): CommandSink, floor_, light_, setup()

### Community 17 - "SensorSnapshot"
Cohesion: 0.22
Nodes (9): SensorSnapshot, airHumPct, airTempC, airValid, floorSampleMs, floorTempC, floorValid, lastMotionMs (+1 more)

### Community 19 - "3. Текущее состояние"
Cohesion: 0.29
Nodes (4): 3. Текущее состояние, ArduinoPwm, cie(), main()

### Community 24 - "SensorConfig"
Cohesion: 0.09
Nodes (19): Block 1 gate — STOP, Block 4 gate — STOP, Block 5 gate — STOP, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки, Task 5.1: Формирование JSON, Task 5.3: Сквозная проверка и документация, Блок 1. Инфраструктура (+11 more)

## Knowledge Gaps
- **89 isolated node(s):** `ssid_`, `wifiPassword_`, `mqttUser_`, `mqttPassword_`, `clientId_` (+84 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 239 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **8 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `LightController` connect `LightController` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `LightController.cpp`, `CommandSink`, `3. Текущее состояние`?**
  _High betweenness centrality (0.206) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `ssid_`, `wifiPassword_`, `mqttUser_` to the rest of the system?**
  _89 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.06050420168067227 - nodes in this community are weakly interconnected._
- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `SensorManager`, `FloorThermostat.cpp`, `CommandSink`, `3. Текущее состояние`?**
  _High betweenness centrality (0.198) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `NetworkManager` (e.g. with `5. Архитектура` and `Task 5.2: `NetworkManager` и MQTT`) actually correct?**
  _`NetworkManager` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07396870554765292 - nodes in this community are weakly interconnected._