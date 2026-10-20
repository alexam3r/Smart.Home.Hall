# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 12 files · ~12,078 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 115 nodes · 125 edges · 10 communities (8 shown, 2 thin omitted)
- Extraction: 97% EXTRACTED · 3% INFERRED · 0% AMBIGUOUS · INFERRED: 4 edges (avg confidence: 0.95)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `e628a185`
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
- test_smoke.cpp
- Блок 3. Свет и CIE1931

## God Nodes (most connected - your core abstractions)
1. `Зависания анимации на 1–2 с: анализ причин` - 10 edges
2. `Smart Home Hall — умный коридор на ESP8266` - 8 edges
3. `loop()` - 7 edges
4. `Project: Smart Corridor ("Argon" Refactoring)` - 7 edges
5. `updateFloorLogic()` - 6 edges
6. `Блок 5. Сеть, MQTT и телеметрия` - 6 edges
7. `MQTT API` - 5 edges
8. `Блок 3. Свет и CIE1931` - 5 edges
9. `setLight()` - 4 edges
10. `callback()` - 4 edges

## Surprising Connections (you probably didn't know these)
- `3. Текущее состояние` --references--> `main()`  [INFERRED]
  AGENTS.md → test/test_smoke/test_smoke.cpp
- `1. Краткий вывод` --references--> `updateFloorLogic()`  [INFERRED]
  docs/freeze-analysis.md → docs/main.cpp
- `4. Почему «иногда» и почему во время розжига` --references--> `updateFloorLogic()`  [INFERRED]
  docs/freeze-analysis.md → docs/main.cpp
- `2. Что именно блокирует `loop()`` --references--> `reconnect()`  [INFERRED]
  docs/freeze-analysis.md → docs/main.cpp

## Import Cycles
- None detected.

## Communities (10 total, 2 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.13
Nodes (14): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+6 more)

### Community 1 - "main.cpp"
Cohesion: 0.13
Nodes (11): 1. Краткий вывод, 2. Что именно блокирует `loop()`, 4. Почему «иногда» и почему во время розжига, callback(), cie1931(), loop(), publishFloorHeatState(), publishInitialStates() (+3 more)

### Community 2 - "Smart Home Hall — умный коридор на ESP8266"
Cohesion: 0.14
Nodes (14): MQTT API, Smart Home Hall — умный коридор на ESP8266, Возможности, Документация, Железо, Команды (контроллер подписан, без retain), Рефакторинг, Сборка и прошивка (+6 more)

### Community 3 - "Зависания анимации на 1–2 с: анализ причин"
Cohesion: 0.29
Nodes (7): 3. Как это выглядит во времени, 5. Проверка гипотез из PRD, 6. Целевое решение (эскиз, не финальный план), 7. Как подтвердить на железе, 8. Вопросы, влияющие на реализацию, 9. Решения владельца (2026-10-20), Зависания анимации на 1–2 с: анализ причин

### Community 4 - "Project: Smart Corridor ("Argon" Refactoring)"
Cohesion: 0.22
Nodes (9): 1. Контекст и Цель проекта, 2. Аппаратная платформа (Hardware Stack — без изменений), 3.1. Модульность и Безопасность, 3.2. Известная проблема (БАГ ДЛЯ ИССЛЕДОВАНИЯ), 3. Программные требования и Архитектура (Что меняем), 4. Бизнес-логика и «Грабли» (Сохранить из `docs/main.cpp`!), 5. Правила работы (Workflow: Superpowers + Graphify + Git), 6. Уточнения, принятые по ходу рефакторинга (+1 more)

### Community 6 - "2026-10-20-argon-refactoring.md"
Cohesion: 0.12
Nodes (16): Block 1 gate — STOP, Block 2 gate — STOP, Block 4 gate — STOP, File Structure, Global Constraints, Review Focus, Task 1.1: Native-тесты и Config, Task 1.2: Секреты и скелет прошивки (+8 more)

### Community 7 - "Блок 5. Сеть, MQTT и телеметрия"
Cohesion: 0.33
Nodes (6): Block 5 gate — STOP, Task 5.1: Формирование JSON, Task 5.2: `NetworkManager` и MQTT, Task 5.3: Сквозная проверка и документация, Блок 5. Сеть, MQTT и телеметрия, Примечания по реализации

### Community 9 - "Блок 3. Свет и CIE1931"
Cohesion: 0.40
Nodes (5): Block 3 gate — STOP, Task 3.1: Таблица CIE1931, Task 3.2: `LightController`, Task 3.3: PWM-адаптер и интеграция света, Блок 3. Свет и CIE1931

## Knowledge Gaps
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Зависания анимации на 1–2 с: анализ причин` connect `Зависания анимации на 1–2 с: анализ причин` to `main.cpp`, `README.md`?**
  _High betweenness centrality (0.397) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `updateFloorLogic()` (e.g. with `1. Краткий вывод` and `4. Почему «иногда» и почему во время розжига`) actually correct?**
  _`updateFloorLogic()` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.13333333333333333 - nodes in this community are weakly interconnected._
- **Why does `Smart Home Hall — умный коридор на ESP8266` connect `Smart Home Hall — умный коридор на ESP8266` to `README.md`?**
  _High betweenness centrality (0.214) - this node is a cross-community bridge._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.12554112554112554 - nodes in this community are weakly interconnected._
- **Why does `3. Текущее состояние` connect `test_smoke.cpp` to `AGENTS.md`?**
  _High betweenness centrality (0.190) - this node is a cross-community bridge._
- **Should `Smart Home Hall — умный коридор на ESP8266` be split into smaller, more focused modules?**
  _Cohesion score 0.14285714285714285 - nodes in this community are weakly interconnected._