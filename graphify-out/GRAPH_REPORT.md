# Graph Report - Smart.Home.Hall  (2026-10-20)

## Corpus Check
- 7 files · ~7,598 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 3 file(s) not represented in the graph (top: (none) 1, .gbr 1, .ini 1)

## Summary
- 74 nodes · 95 edges · 6 communities (5 shown, 1 thin omitted)
- Extraction: 85% EXTRACTED · 15% INFERRED · 0% AMBIGUOUS · INFERRED: 14 edges (avg confidence: 0.95)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `530aaaca`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- AGENTS.md
- main.cpp
- Smart Home Hall — умный коридор на ESP8266
- Зависания анимации на 1–2 с: анализ причин
- Project: Smart Corridor ("Argon" Refactoring)
- README.md

## God Nodes (most connected - your core abstractions)
1. `loop()` - 18 edges
2. `Зависания анимации на 1–2 с: анализ причин` - 10 edges
3. `Smart Home Hall — умный коридор на ESP8266` - 8 edges
4. `Project: Smart Corridor ("Argon" Refactoring)` - 7 edges
5. `updateFloorLogic()` - 6 edges
6. `MQTT API` - 5 edges
7. `setLight()` - 4 edges
8. `callback()` - 4 edges
9. `reconnect()` - 4 edges
10. `3. Программные требования и Архитектура (Что меняем)` - 3 edges

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

## Communities (6 total, 1 thin omitted)

### Community 0 - "AGENTS.md"
Cohesion: 0.15
Nodes (16): 10. Соглашения по коду, 11. Безопасность, 12. Проверка и Definition of Done, 13. Git, 14. Запрещено, 15. Принятые решения, 1. Что за проект, 2. Источники правды (+8 more)

### Community 1 - "main.cpp"
Cohesion: 0.13
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

## Knowledge Gaps
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `loop()` connect `AGENTS.md` to `main.cpp`, `Smart Home Hall — умный коридор на ESP8266`, `Зависания анимации на 1–2 с: анализ причин`, `Project: Smart Corridor ("Argon" Refactoring)`?**
  _High betweenness centrality (0.551) - this node is a cross-community bridge._
- **Are the 11 inferred relationships involving `loop()` (e.g. with `10. Соглашения по коду` and `12. Проверка и Definition of Done`) actually correct?**
  _`loop()` has 11 INFERRED edges - model-reasoned connections that need verification._
- **Should `AGENTS.md` be split into smaller, more focused modules?**
  _Cohesion score 0.14705882352941177 - nodes in this community are weakly interconnected._
- **Why does `Smart Home Hall — умный коридор на ESP8266` connect `Smart Home Hall — умный коридор на ESP8266` to `README.md`?**
  _High betweenness centrality (0.206) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `updateFloorLogic()` (e.g. with `1. Краткий вывод` and `4. Почему «иногда» и почему во время розжига`) actually correct?**
  _`updateFloorLogic()` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `main.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.1323529411764706 - nodes in this community are weakly interconnected._
- **Why does `Зависания анимации на 1–2 с: анализ причин` connect `Зависания анимации на 1–2 с: анализ причин` to `main.cpp`, `README.md`?**
  _High betweenness centrality (0.178) - this node is a cross-community bridge._