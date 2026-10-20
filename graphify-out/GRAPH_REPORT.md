# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 39 files · ~18,096 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 469 nodes · 753 edges · 17 communities
- Extraction: 77% EXTRACTED · 23% INFERRED · 0% AMBIGUOUS · INFERRED: 170 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `25676f1a`
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
- Project: Smart Corridor ("Argon" Refactoring)
- FakeBus
- NetworkManager.cpp
- 3. Текущее состояние
- FloorThermostat.cpp
- FloorThermostat.cpp
- LightController.cpp
- SensorSnapshot

## God Nodes (most connected - your core abstractions)
1. `LightController` - 37 edges
2. `NetworkManager` - 35 edges
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

## Communities (17 total, 0 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.06
Nodes (30): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+22 more)

### Community 1 - "main.cpp"
Cohesion: 0.07
Nodes (22): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию (+14 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.07
Nodes (25): Claude Code, Правила, С чего начать после `git clone`, Текущий статус, Замечания, Испытания на железе, После испытаний, Проверено (+17 more)

### Community 3 - "SensorManager"
Cohesion: 0.08
Nodes (30): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Task 4.2: Реле и интеграция пола, Task 5.2: `NetworkManager` и MQTT, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle (+22 more)

### Community 4 - "LightController"
Cohesion: 0.11
Nodes (24): Task 3.2: `LightController`, LightController, config_, current_, fadeStartMs_, fading_, lastActivityMs_, lastStepMs_ (+16 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (26): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+18 more)

### Community 6 - "NetworkManager"
Cohesion: 0.07
Nodes (19): NetworkManager, attempted_, client_, clientId_, initialPublished_, instance_, lastAttemptMs_, lastWifiAttemptMs_ (+11 more)

### Community 7 - "CommandSink"
Cohesion: 0.10
Nodes (6): IMqttCommandSink, CommandSink, floor_, light_, setup(), NetworkManager::NetworkManager()

### Community 8 - "LightController.cpp"
Cohesion: 0.08
Nodes (18): ILightObserver, IPwmOutput, LightConfig, defaultBrightness, fadeMs, motionTimeoutMs, pwmMax, safetyTimeoutMs (+10 more)

### Community 9 - "Project: Smart Corridor ("Argon" Refactoring)"
Cohesion: 0.22
Nodes (8): 1. Контекст и Цель проекта, 2. Аппаратная платформа (Hardware Stack — без изменений), 3.1. Модульность и Безопасность, 3.2. Известная проблема (БАГ ДЛЯ ИССЛЕДОВАНИЯ), 3. Программные требования и Архитектура (Что меняем), 5. Правила работы (Workflow: Superpowers + Graphify + Git), 6. Уточнения, принятые по ходу рефакторинга, Project: Smart Corridor ("Argon" Refactoring)

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 11 - "NetworkManager.cpp"
Cohesion: 0.06
Nodes (5): loop(), publishTelemetry(), NetworkManager::callbackTrampoline(), NetworkManager::handleMessage(), NetworkManager::update()

### Community 12 - "3. Текущее состояние"
Cohesion: 0.18
Nodes (8): 3. Текущее состояние, Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, ArduinoPwm, cie(), main()

### Community 13 - "FloorThermostat.cpp"
Cohesion: 0.20
Nodes (8): FloorThermostat::onMotion(), FloorThermostat::onSample(), FloorThermostat::onSensorInvalid(), FloorThermostat::recomputeHeat(), FloorThermostat::setMaster(), FloorThermostat::setMotionMode(), FloorThermostat::setTarget(), FloorThermostat::update()

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.10
Nodes (13): FloorConfig, defaultTargetC, hysteresisC, motionTimeoutMs, safetyTimeoutMs, staleTimeoutMs, IFloorObserver, IRelayOutput (+5 more)

### Community 15 - "LightController.cpp"
Cohesion: 0.07
Nodes (10): cie1931(), LightController::applyAutoOff(), LightController::dutyFor(), LightController::onMotion(), LightController::setBrightness(), LightController::setPower(), formatTelemetry(), truncToTenths() (+2 more)

### Community 24 - "SensorSnapshot"
Cohesion: 0.08
Nodes (17): ISensorBus, SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, SensorSnapshot, airHumPct (+9 more)

## Knowledge Gaps
- **90 isolated node(s):** `ssid_`, `wifiPassword_`, `mqttUser_`, `mqttPassword_`, `clientId_` (+85 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 245 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `SensorManager`, `CommandSink`, `Project: Smart Corridor ("Argon" Refactoring)`, `3. Текущее состояние`, `FloorThermostat.cpp`, `FloorThermostat.cpp`?**
  _High betweenness centrality (0.205) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `ssid_`, `wifiPassword_`, `mqttUser_` to the rest of the system?**
  _90 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.06439393939393939 - nodes in this community are weakly interconnected._
- **Why does `LightController` connect `LightController` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `CommandSink`, `LightController.cpp`, `Project: Smart Corridor ("Argon" Refactoring)`, `3. Текущее состояние`, `LightController.cpp`?**
  _High betweenness centrality (0.204) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `NetworkManager` (e.g. with `5. Архитектура` and `Task 5.2: `NetworkManager` и MQTT`) actually correct?**
  _`NetworkManager` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07396870554765292 - nodes in this community are weakly interconnected._