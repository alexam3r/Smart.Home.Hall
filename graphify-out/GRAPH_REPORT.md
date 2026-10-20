# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 38 files · ~17,175 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 463 nodes · 745 edges · 21 communities (15 shown, 6 thin omitted)
- Extraction: 77% EXTRACTED · 23% INFERRED · 0% AMBIGUOUS · INFERRED: 170 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `97b1cf23`
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
- SerialLightObserver
- FloorThermostat.cpp
- FloorThermostat.cpp
- src/main.cpp
- 7. Как подтвердить на железе
- IMqttCommandSink
- SerialFloorObserver
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

## Communities (21 total, 6 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.05
Nodes (30): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+22 more)

### Community 1 - "main.cpp"
Cohesion: 0.09
Nodes (21): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 8. Вопросы, влияющие на реализацию, 9. Решения владельца (2026-10-20) (+13 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.12
Nodes (16): MQTT API, Smart Home Hall — умный коридор на ESP8266, Архитектура, Возможности, Для AI-агентов, Документация, Железо, Команды (контроллер подписан, без retain) (+8 more)

### Community 3 - "SensorManager"
Cohesion: 0.09
Nodes (29): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Task 5.2: `NetworkManager` и MQTT, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle, Waiting (+21 more)

### Community 4 - "LightController"
Cohesion: 0.10
Nodes (28): Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.2: `LightController`, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, LightController, config_, current_ (+20 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (26): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+18 more)

### Community 6 - "NetworkManager"
Cohesion: 0.10
Nodes (15): NetworkManager, attempted_, client_, clientId_, initialPublished_, instance_, lastAttemptMs_, lastWifiAttemptMs_ (+7 more)

### Community 7 - "NetworkManager.cpp"
Cohesion: 0.15
Nodes (3): NetworkManager::callbackTrampoline(), NetworkManager::handleMessage(), NetworkManager::update()

### Community 8 - "LightController.cpp"
Cohesion: 0.05
Nodes (25): ILightObserver, IPwmOutput, LightConfig, defaultBrightness, fadeMs, motionTimeoutMs, pwmMax, safetyTimeoutMs (+17 more)

### Community 9 - "План реализации рефакторинга Argon"
Cohesion: 0.50
Nodes (4): File Structure, Global Constraints, Review Focus, План реализации рефакторинга Argon

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 13 - "FloorThermostat.cpp"
Cohesion: 0.20
Nodes (8): FloorThermostat::onMotion(), FloorThermostat::onSample(), FloorThermostat::onSensorInvalid(), FloorThermostat::recomputeHeat(), FloorThermostat::setMaster(), FloorThermostat::setMotionMode(), FloorThermostat::setTarget(), FloorThermostat::update()

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.06
Nodes (25): Block 1 gate — STOP, Block 4 gate — STOP, Block 5 gate — STOP, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки, Task 4.2: Реле и интеграция пола, Task 5.1: Формирование JSON, Task 5.3: Сквозная проверка и документация (+17 more)

### Community 15 - "src/main.cpp"
Cohesion: 0.07
Nodes (10): formatTelemetry(), truncToTenths(), CommandSink, floor_, light_, loop(), publishTelemetry(), setup() (+2 more)

### Community 24 - "SensorSnapshot"
Cohesion: 0.08
Nodes (17): ISensorBus, SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, SensorSnapshot, airHumPct (+9 more)

## Knowledge Gaps
- **90 isolated node(s):** `ssid_`, `wifiPassword_`, `mqttUser_`, `mqttPassword_`, `clientId_` (+85 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 241 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `FloorThermostat.cpp`, `FloorThermostat.cpp`, `src/main.cpp`?**
  _High betweenness centrality (0.206) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `ssid_`, `wifiPassword_`, `mqttUser_` to the rest of the system?**
  _90 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.05384615384615385 - nodes in this community are weakly interconnected._
- **Why does `LightController` connect `LightController` to `AGENTS.md`, `LightController.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `src/main.cpp`?**
  _High betweenness centrality (0.205) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `NetworkManager` (e.g. with `5. Архитектура` and `Task 5.2: `NetworkManager` и MQTT`) actually correct?**
  _`NetworkManager` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.0855614973262032 - nodes in this community are weakly interconnected._