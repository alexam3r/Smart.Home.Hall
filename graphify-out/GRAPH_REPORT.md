# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 39 files · ~18,312 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 478 nodes · 766 edges · 16 communities
- Extraction: 77% EXTRACTED · 23% INFERRED · 0% AMBIGUOUS · INFERRED: 174 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `d02db27f`
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
- src/main.cpp
- LightController.cpp
- FloorThermostat.cpp
- FakeBus
- NetworkManager.cpp
- 3. Текущее состояние
- ILightObserver
- FloorThermostat.cpp
- ISensorBus

## God Nodes (most connected - your core abstractions)
1. `LightController` - 37 edges
2. `NetworkManager` - 36 edges
3. `FloorThermostat` - 32 edges
4. `SensorManager` - 27 edges
5. `ArduinoSensors` - 16 edges
6. `FakeBus` - 15 edges
7. `CommandSink` - 14 edges
8. `IMqttCommandSink` - 13 edges
9. `ISensorBus` - 12 edges
10. `SensorSnapshot` - 12 edges

## Surprising Connections (you probably didn't know these)
- `6. Целевое решение (эскиз, не финальный план)` --references--> `FloorThermostat`  [INFERRED]
  docs/freeze-analysis.md → include/core/FloorThermostat.h
- `Task 5.2: `NetworkManager` и MQTT` --references--> `IMqttCommandSink`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/NetworkManager.h
- `5. Архитектура` --references--> `NetworkManager`  [INFERRED]
  AGENTS.md → include/NetworkManager.h
- `Task 5.2: `NetworkManager` и MQTT` --references--> `NetworkManager`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/NetworkManager.h
- `3.1. Модульность и Безопасность` --references--> `NetworkManager`  [INFERRED]
  PRD.md → include/NetworkManager.h

## Import Cycles
- None detected.

## Communities (16 total, 0 thin omitted)

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
Cohesion: 0.08
Nodes (33): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Task 4.2: Реле и интеграция пола, Task 5.2: `NetworkManager` и MQTT, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle (+25 more)

### Community 4 - "LightController"
Cohesion: 0.10
Nodes (28): Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.2: `LightController`, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, LightController, config_, current_ (+20 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (26): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+18 more)

### Community 6 - "NetworkManager"
Cohesion: 0.05
Nodes (26): File Structure, Global Constraints, Review Focus, План реализации рефакторинга Argon, IMqttCommandSink, NetworkManager, attempted_, client_ (+18 more)

### Community 7 - "src/main.cpp"
Cohesion: 0.10
Nodes (10): formatTelemetry(), truncToTenths(), CommandSink, floor_, light_, loop(), publishTelemetry(), setup() (+2 more)

### Community 8 - "LightController.cpp"
Cohesion: 0.06
Nodes (22): 3. Текущее состояние, IPwmOutput, LightConfig, defaultBrightness, fadeMs, motionTimeoutMs, pwmMax, safetyTimeoutMs (+14 more)

### Community 9 - "FloorThermostat.cpp"
Cohesion: 0.20
Nodes (8): FloorThermostat::onMotion(), FloorThermostat::onSample(), FloorThermostat::onSensorInvalid(), FloorThermostat::recomputeHeat(), FloorThermostat::setMaster(), FloorThermostat::setMotionMode(), FloorThermostat::setTarget(), FloorThermostat::update()

### Community 10 - "FakeBus"
Cohesion: 0.14
Nodes (9): FakeBus, airHum, airOk, airReads, airTemp, floorReady, floorRequests, floorTemp (+1 more)

### Community 11 - "NetworkManager.cpp"
Cohesion: 0.05
Nodes (3): NetworkManager::callbackTrampoline(), NetworkManager::handleMessage(), NetworkManager::update()

### Community 12 - "3. Текущее состояние"
Cohesion: 0.17
Nodes (11): Block 1 gate — STOP, Block 4 gate — STOP, Block 5 gate — STOP, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки, Task 5.1: Формирование JSON, Task 5.3: Сквозная проверка и документация, Блок 1. Инфраструктура (+3 more)

### Community 13 - "ILightObserver"
Cohesion: 0.18
Nodes (6): ILightObserver, FakeObserver, on, settledCount, settledPercent, stateChanges

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.10
Nodes (13): FloorConfig, defaultTargetC, hysteresisC, motionTimeoutMs, safetyTimeoutMs, staleTimeoutMs, IFloorObserver, IRelayOutput (+5 more)

### Community 24 - "ISensorBus"
Cohesion: 0.08
Nodes (17): ISensorBus, SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, SensorSnapshot, airHumPct (+9 more)

## Knowledge Gaps
- **93 isolated node(s):** `ssid_`, `wifiPassword_`, `mqttUser_`, `mqttPassword_`, `clientId_` (+88 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 252 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `SensorManager`, `src/main.cpp`, `LightController.cpp`, `FloorThermostat.cpp`, `FloorThermostat.cpp`?**
  _High betweenness centrality (0.201) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `ssid_`, `wifiPassword_`, `mqttUser_` to the rest of the system?**
  _93 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.08333333333333333 - nodes in this community are weakly interconnected._
- **Why does `LightController` connect `LightController` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `src/main.cpp`, `LightController.cpp`, `ILightObserver`?**
  _High betweenness centrality (0.200) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `NetworkManager` (e.g. with `5. Архитектура` and `Task 5.2: `NetworkManager` и MQTT`) actually correct?**
  _`NetworkManager` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07657657657657657 - nodes in this community are weakly interconnected._