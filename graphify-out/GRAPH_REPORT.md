# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 17 files · ~13,121 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 202 nodes · 270 edges · 12 communities (11 shown, 1 thin omitted)
- Extraction: 84% EXTRACTED · 16% INFERRED · 0% AMBIGUOUS · INFERRED: 42 edges (avg confidence: 0.91)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `1deb2642`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- AGENTS.md
- main.cpp
- Smart Home Hall — умный коридор на ESP8266
- ISensorBus
- Project: Smart Corridor ("Argon" Refactoring)
- SensorManager
- 2026-10-20-argon-refactoring.md
- test_sensors.cpp
- ArduinoSensors
- FakeBus
- SensorSnapshot

## God Nodes (most connected - your core abstractions)
1. `SensorManager` - 27 edges
2. `ArduinoSensors` - 16 edges
3. `FakeBus` - 13 edges
4. `ISensorBus` - 11 edges
5. `SensorSnapshot` - 10 edges
6. `Smart Home Hall — умный коридор на ESP8266` - 10 edges
7. `Зависания анимации на 1–2 с: анализ причин` - 10 edges
8. `SensorConfig` - 8 edges
9. `loop()` - 7 edges
10. `snapshot_` - 7 edges

## Surprising Connections (you probably didn't know these)
- `Примечания по реализации` --references--> `SensorConfig`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/core/SensorManager.h
- `5. Целевая архитектура` --references--> `SensorManager`  [INFERRED]
  AGENTS.md → include/core/SensorManager.h
- `3.1. Модульность и Безопасность` --references--> `SensorManager`  [INFERRED]
  PRD.md → include/core/SensorManager.h
- `Рефакторинг` --references--> `SensorManager`  [INFERRED]
  README.md → include/core/SensorManager.h
- `План реализации рефакторинга Argon` --references--> `ISensorBus`  [INFERRED]
  docs/superpowers/plans/2026-10-20-argon-refactoring.md → include/core/SensorManager.h

## Import Cycles
- None detected.

## Communities (12 total, 1 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.09
Nodes (15): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+7 more)

### Community 1 - "main.cpp"
Cohesion: 0.11
Nodes (18): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию (+10 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.14
Nodes (14): MQTT API, Smart Home Hall — умный коридор на ESP8266, Возможности, Документация, Железо, Команды (контроллер подписан, без retain), Рефакторинг, Сборка и прошивка (+6 more)

### Community 3 - "ISensorBus"
Cohesion: 0.12
Nodes (9): ISensorBus, SensorConfig, airIntervalMs, floorConversionMs, floorIntervalMs, pirWarmupMs, isValidFloorTemp(), SensorManager::SensorManager() (+1 more)

### Community 4 - "Project: Smart Corridor ("Argon" Refactoring)"
Cohesion: 0.22
Nodes (9): 1. Контекст и Цель проекта, 2. Аппаратная платформа (Hardware Stack — без изменений), 3.1. Модульность и Безопасность, 3.2. Известная проблема (БАГ ДЛЯ ИССЛЕДОВАНИЯ), 3. Программные требования и Архитектура (Что меняем), 4. Бизнес-логика и «Грабли» (Сохранить из `docs/main.cpp`!), 5. Правила работы (Workflow: Superpowers + Graphify + Git), 6. Уточнения, принятые по ходу рефакторинга (+1 more)

### Community 5 - "SensorManager"
Cohesion: 0.13
Nodes (15): FloorState, Idle, Waiting, SensorManager, airInitialized_, bootMs_, config_, floorRequested_ (+7 more)

### Community 6 - "2026-10-20-argon-refactoring.md"
Cohesion: 0.08
Nodes (27): Block 1 gate — STOP, Block 2 gate — STOP, Block 3 gate — STOP, Block 4 gate — STOP, Block 5 gate — STOP, File Structure, Global Constraints, Review Focus (+19 more)

### Community 7 - "test_sensors.cpp"
Cohesion: 0.29
Nodes (9): snapshot_, test_air_failure_invalidates_and_recovers(), test_first_update_requests_floor_and_reads_air(), test_floor_reads_after_conversion_time(), test_invalid_floor_reading_keeps_last_valid(), test_invalid_floor_values_are_rejected(), test_pir_high_at_boot_does_not_trigger_after_warmup(), test_pir_warmup_and_edge_latching() (+1 more)

### Community 9 - "ArduinoSensors"
Cohesion: 0.19
Nodes (5): 3. Текущее состояние, ArduinoSensors, am2320_, floorSensors_, oneWire_

### Community 10 - "FakeBus"
Cohesion: 0.17
Nodes (8): FakeBus, airHum, airOk, airReads, airTemp, floorRequests, floorTemp, pir

### Community 11 - "SensorSnapshot"
Cohesion: 0.22
Nodes (9): SensorSnapshot, airHumPct, airTempC, airValid, floorSampleMs, floorTempC, floorValid, lastMotionMs (+1 more)

## Knowledge Gaps
- **35 isolated node(s):** `floorIntervalMs`, `floorConversionMs`, `airIntervalMs`, `pirWarmupMs`, `floorValid` (+30 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 119 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `SensorManager` connect `SensorManager` to `AGENTS.md`, `Smart Home Hall — умный коридор на ESP8266`, `ISensorBus`, `Project: Smart Corridor ("Argon" Refactoring)`, `2026-10-20-argon-refactoring.md`, `test_sensors.cpp`, `ArduinoSensors`, `SensorSnapshot`?**
  _High betweenness centrality (0.384) - this node is a cross-community bridge._
- **Are the 7 inferred relationships involving `SensorManager` (e.g. with `3. Текущее состояние` and `5. Целевая архитектура`) actually correct?**
  _`SensorManager` has 7 INFERRED edges - model-reasoned connections that need verification._
- **What connects `floorIntervalMs`, `floorConversionMs`, `airIntervalMs` to the rest of the system?**
  _35 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.09090909090909091 - nodes in this community are weakly interconnected._
- **Why does `ArduinoSensors` connect `ArduinoSensors` to `Smart Home Hall — умный коридор на ESP8266`, `ISensorBus`, `2026-10-20-argon-refactoring.md`?**
  _High betweenness centrality (0.243) - this node is a cross-community bridge._
- **Are the 3 inferred relationships involving `ArduinoSensors` (e.g. with `3. Текущее состояние` and `Task 2.2: Аппаратный адаптер датчиков и интеграция`) actually correct?**
  _`ArduinoSensors` has 3 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.11333333333333333 - nodes in this community are weakly interconnected._