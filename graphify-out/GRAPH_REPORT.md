# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 39 files · ~17,936 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 468 nodes · 751 edges · 19 communities
- Extraction: 77% EXTRACTED · 23% INFERRED · 0% AMBIGUOUS · INFERRED: 170 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `9ff55c0f`
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
- .brightness
- LightController.cpp
- Project: Smart Corridor ("Argon" Refactoring)
- FakeBus
- NetworkManager.cpp
- README.md
- FloorThermostat.cpp
- FloorThermostat.cpp
- src/main.cpp
- 2026-10-20-argon-refactoring.md
- Claude Code
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
- `5. Архитектура` --references--> `NetworkManager`  [INFERRED]
  AGENTS.md → include/NetworkManager.h
- `Task 5.2: `NetworkManager` и MQTT` --references--> `NetworkManager`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/NetworkManager.h
- `3.1. Модульность и Безопасность` --references--> `NetworkManager`  [INFERRED]
  PRD.md → include/NetworkManager.h

## Import Cycles
- None detected.

## Communities (19 total, 0 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.15
Nodes (12): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 1. Что за проект, 2. Источники правды, 4. Команды (+4 more)

### Community 1 - "main.cpp"
Cohesion: 0.07
Nodes (25): 3. Текущее состояние, 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе (+17 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.12
Nodes (17): Home Assistant, MQTT API, Smart Home Hall — умный коридор на ESP8266, Архитектура, Возможности, Для AI-агентов, Документация, Железо (+9 more)

### Community 3 - "SensorManager"
Cohesion: 0.08
Nodes (30): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Task 4.2: Реле и интеграция пола, Task 5.2: `NetworkManager` и MQTT, Блок 2. Датчики и неблокирующий опрос, FloorState, Idle (+22 more)

### Community 4 - "LightController"
Cohesion: 0.10
Nodes (28): Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.2: `LightController`, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931, LightController, config_, current_ (+20 more)

### Community 5 - "FloorThermostat"
Cohesion: 0.12
Nodes (26): Task 4.1: `FloorThermostat`, FloorThermostat, config_, floorTempC_, hasSample_, lastActivityMs_, lastValidSampleMs_, FakeObserver (+18 more)

### Community 6 - "NetworkManager"
Cohesion: 0.05
Nodes (25): File Structure, Global Constraints, Review Focus, План реализации рефакторинга Argon, IMqttCommandSink, NetworkManager, attempted_, client_ (+17 more)

### Community 7 - ".brightness"
Cohesion: 0.22
Nodes (8): 15. Принятые решения, 8. Инварианты поведения (не сломать при рефакторинге), Block 5 gate — STOP, Task 5.1: Формирование JSON, Task 5.3: Сквозная проверка и документация, Блок 5. Сеть, MQTT и телеметрия, Примечания по реализации, 4. Бизнес-логика и «Грабли» (Сохранить из `docs/main.cpp`!)

### Community 8 - "LightController.cpp"
Cohesion: 0.05
Nodes (25): ILightObserver, IPwmOutput, LightConfig, defaultBrightness, fadeMs, motionTimeoutMs, pwmMax, safetyTimeoutMs (+17 more)

### Community 9 - "Project: Smart Corridor ("Argon" Refactoring)"
Cohesion: 0.22
Nodes (8): 1. Контекст и Цель проекта, 2. Аппаратная платформа (Hardware Stack — без изменений), 3.1. Модульность и Безопасность, 3.2. Известная проблема (БАГ ДЛЯ ИССЛЕДОВАНИЯ), 3. Программные требования и Архитектура (Что меняем), 5. Правила работы (Workflow: Superpowers + Graphify + Git), 6. Уточнения, принятые по ходу рефакторинга, Project: Smart Corridor ("Argon" Refactoring)

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 11 - "NetworkManager.cpp"
Cohesion: 0.05
Nodes (3): NetworkManager::callbackTrampoline(), NetworkManager::handleMessage(), NetworkManager::update()

### Community 12 - "README.md"
Cohesion: 0.33
Nodes (3): Замечания, Испытания на железе, Проверено

### Community 13 - "FloorThermostat.cpp"
Cohesion: 0.20
Nodes (8): FloorThermostat::onMotion(), FloorThermostat::onSample(), FloorThermostat::onSensorInvalid(), FloorThermostat::recomputeHeat(), FloorThermostat::setMaster(), FloorThermostat::setMotionMode(), FloorThermostat::setTarget(), FloorThermostat::update()

### Community 14 - "FloorThermostat.cpp"
Cohesion: 0.10
Nodes (13): FloorConfig, defaultTargetC, hysteresisC, motionTimeoutMs, safetyTimeoutMs, staleTimeoutMs, IFloorObserver, IRelayOutput (+5 more)

### Community 15 - "src/main.cpp"
Cohesion: 0.10
Nodes (10): formatTelemetry(), truncToTenths(), CommandSink, floor_, light_, loop(), publishTelemetry(), setup() (+2 more)

### Community 16 - "2026-10-20-argon-refactoring.md"
Cohesion: 0.29
Nodes (6): Block 1 gate — STOP, Block 4 gate — STOP, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки, Блок 1. Инфраструктура, Блок 4. Тёплый пол

### Community 17 - "Claude Code"
Cohesion: 0.33
Nodes (4): Claude Code, Правила, С чего начать после `git clone`, Текущий статус

### Community 24 - "SensorSnapshot"
Cohesion: 0.08
Nodes (17): ISensorBus, SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, SensorSnapshot, airHumPct (+9 more)

## Knowledge Gaps
- **90 isolated node(s):** `ssid_`, `wifiPassword_`, `mqttUser_`, `mqttPassword_`, `clientId_` (+85 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 244 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `FloorThermostat` connect `FloorThermostat` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `SensorManager`, `Project: Smart Corridor ("Argon" Refactoring)`, `FloorThermostat.cpp`, `FloorThermostat.cpp`, `src/main.cpp`?**
  _High betweenness centrality (0.205) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `LightController` (e.g. with `3. Текущее состояние` and `5. Архитектура`) actually correct?**
  _`LightController` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `ssid_`, `wifiPassword_`, `mqttUser_` to the rest of the system?**
  _90 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.06829268292682927 - nodes in this community are weakly interconnected._
- **Why does `LightController` connect `LightController` to `AGENTS.md`, `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `.brightness`, `LightController.cpp`, `Project: Smart Corridor ("Argon" Refactoring)`, `src/main.cpp`?**
  _High betweenness centrality (0.204) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `NetworkManager` (e.g. with `5. Архитектура` and `Task 5.2: `NetworkManager` и MQTT`) actually correct?**
  _`NetworkManager` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `Smart Home Hall — умный коридор на ESP8266` be split into smaller, more focused modules?**
  _Cohesion score 0.11764705882352941 - nodes in this community are weakly interconnected._