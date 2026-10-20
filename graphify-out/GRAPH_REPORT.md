# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 8 files · ~11,658 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 102 nodes · 131 edges · 9 communities (8 shown, 1 thin omitted)
- Extraction: 82% EXTRACTED · 18% INFERRED · 0% AMBIGUOUS · INFERRED: 23 edges (avg confidence: 0.95)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `02ec5ff7`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- AGENTS.md
- main.cpp
- Smart Home Hall — умный коридор на ESP8266
- Зависания анимации на 1–2 с: анализ причин
- Project: Smart Corridor ("Argon" Refactoring)
- README.md
- 2026-10-20-argon-refactoring.md
- Блок 5. Сеть, MQTT и телеметрия
- Блок 2. Датчики и неблокирующий опрос

## God Nodes (most connected - your core abstractions)
1. `loop()` - 26 edges
2. `Зависания анимации на 1–2 с: анализ причин` - 10 edges
3. `Smart Home Hall — умный коридор на ESP8266` - 8 edges
4. `Project: Smart Corridor ("Argon" Refactoring)` - 7 edges
5. `updateFloorLogic()` - 6 edges
6. `Блок 5. Сеть, MQTT и телеметрия` - 6 edges
7. `MQTT API` - 5 edges
8. `План реализации рефакторинга Argon` - 5 edges
9. `Блок 3. Свет и CIE1931` - 5 edges
10. `setLight()` - 4 edges

## Surprising Connections (you probably didn't know these)
- `3.2. Известная проблема (БАГ ДЛЯ ИССЛЕДОВАНИЯ)` --references--> `loop()`  [INFERRED]
  PRD.md → docs/main.cpp
- `6. Уточнения, принятые по ходу рефакторинга` --references--> `loop()`  [INFERRED]
  PRD.md → docs/main.cpp
- `Тайминги` --references--> `loop()`  [INFERRED]
  README.md → docs/main.cpp
- `10. Соглашения по коду` --references--> `loop()`  [INFERRED]
  AGENTS.md → docs/main.cpp
- `12. Проверка и Definition of Done` --references--> `loop()`  [INFERRED]
  AGENTS.md → docs/main.cpp

## Import Cycles
- None detected.

## Communities (9 total, 1 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.12
Nodes (20): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+12 more)

### Community 1 - "main.cpp"
Cohesion: 0.14
Nodes (6): 2. Что именно блокирует `loop()`, callback(), cie1931(), publishInitialStates(), reconnect(), setLight()

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.15
Nodes (13): MQTT API, Smart Home Hall — умный коридор на ESP8266, Возможности, Документация, Железо, Команды (контроллер подписан, без retain), Рефакторинг, Сборка и прошивка (+5 more)

### Community 3 - "Зависания анимации на 1–2 с: анализ причин"
Cohesion: 0.20
Nodes (11): 1. Краткий вывод, 3. Как это выглядит во времени, 4. Почему «иногда» и почему во время розжига, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию, 9. Решения владельца (2026-10-20) (+3 more)

### Community 4 - "Project: Smart Corridor ("Argon" Refactoring)"
Cohesion: 0.22
Nodes (9): 1. Контекст и Цель проекта, 2. Аппаратная платформа (Hardware Stack — без изменений), 3.1. Модульность и Безопасность, 3.2. Известная проблема (БАГ ДЛЯ ИССЛЕДОВАНИЯ), 3. Программные требования и Архитектура (Что меняем), 4. Бизнес-логика и «Грабли» (Сохранить из `docs/main.cpp`!), 5. Правила работы (Workflow: Superpowers + Graphify + Git), 6. Уточнения, принятые по ходу рефакторинга (+1 more)

### Community 6 - "2026-10-20-argon-refactoring.md"
Cohesion: 0.13
Nodes (14): setup(), Block 1 gate — STOP, Block 3 gate — STOP, Block 4 gate — STOP, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки, Task 3.1: Таблица CIE1931, Task 3.2: `LightController` (+6 more)

### Community 7 - "Блок 5. Сеть, MQTT и телеметрия"
Cohesion: 0.33
Nodes (6): Block 5 gate — STOP, Task 5.1: Формирование JSON, Task 5.2: `NetworkManager` и MQTT, Task 5.3: Сквозная проверка и документация, Блок 5. Сеть, MQTT и телеметрия, Примечания по реализации

### Community 8 - "Блок 2. Датчики и неблокирующий опрос"
Cohesion: 0.50
Nodes (4): Block 2 gate — STOP, Task 2.1: `SensorManager` — чистый автомат, Task 2.2: Аппаратный адаптер датчиков и интеграция, Блок 2. Датчики и неблокирующий опрос

## Knowledge Gaps
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `loop()` connect `AGENTS.md` to `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `Зависания анимации на 1–2 с: анализ причин`, `Project: Smart Corridor ("Argon" Refactoring)`, `2026-10-20-argon-refactoring.md`, `Блок 5. Сеть, MQTT и телеметрия`, `Блок 2. Датчики и неблокирующий опрос`?**
  _High betweenness centrality (0.703) - this node is a cross-community bridge._
- **Are the 19 inferred relationships involving `loop()` (e.g. with `10. Соглашения по коду` and `12. Проверка и Definition of Done`) actually correct?**
  _`loop()` has 19 INFERRED edges - model-reasoned connections that need verification._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.12380952380952381 - nodes in this community are weakly interconnected._
- **Why does `Smart Home Hall — умный коридор на ESP8266` connect `Smart Home Hall — умный коридор на ESP8266` to `README.md`?**
  _High betweenness centrality (0.148) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `updateFloorLogic()` (e.g. with `1. Краткий вывод` and `4. Почему «иногда» и почему во время розжига`) actually correct?**
  _`updateFloorLogic()` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.14166666666666666 - nodes in this community are weakly interconnected._
- **Should `2026-10-20-argon-refactoring.md` be split into smaller, more focused modules?**
  _Cohesion score 0.13333333333333333 - nodes in this community are weakly interconnected._