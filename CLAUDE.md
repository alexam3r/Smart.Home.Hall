# CLAUDE.md

@AGENTS.md

## Claude Code

Проектные правила, архитектура, MQTT-контракт и workflow — в `AGENTS.md` (импортирован выше). Этот файл добавляет только то, что специфично для Claude Code.

### С чего начать после `git clone`

1. `README.md` — что за устройство, архитектурная схема, MQTT API, сборка.
2. `AGENTS.md` — правила, инварианты поведения, текущее состояние.
3. `PRD.md` — требования; `docs/freeze-analysis.md` — почему появился рефакторинг; `docs/superpowers/plans/2026-10-20-argon-refactoring.md` — как его делали.
4. `graphify-out/GRAPH_REPORT.md` — карта модулей и связей.

### Текущий статус

- Рефакторинг Argon завершён, код в `main`; ветка `refactor/argon` слита и удалена.
- 40 native-тестов (`pio test -e native`); `pio run` собирает `d1_mini`.
- Прошивка испытана на железе 2026-10-20: все проверенные сценарии в норме, замечаний нет. Проект заморожен (тег `argon-2026-10-20`); новые правки — по запросу.
- `include/Secrets.h` не коммитится. Без него сборка идёт с заглушками из `Secrets.h.example`, но устройство не подключится к Wi-Fi/MQTT.
- `docs/hall.yaml` — рабочий пакет Home Assistant; датчики и climate читают JSON `home/hall/state`. `docs/hardware-test-report.md` — отчёт испытаний.

### Правила

- Не переписывай готовую архитектуру и не начинай рефакторинг заново без запроса.
- Не меняй пины, MQTT-топики, retain, тайминги и порядок ключей JSON. Источник контракта — `README.md`, требования — `PRD.md`.
- Перед изменениями: `pio test -e native` и `pio run`; после изменений — снова оба. Отдельно указывай, что проверено только сборкой.
- Перед проектными решениями используй скиллы Superpowers: `brainstorming`, `writing-plans`, `systematic-debugging`, `verification-before-completion`.
- Секреты — только в `include/Secrets.h` (в `.gitignore`); в репозитории — лишь `Secrets.h.example`.
- Коммиты осмысленные (`feat(light): ...`, `fix(floor): ...`, `docs: ...`); не смешивай несвязанные изменения.
- Если требование неоднозначно или расходится с легаси — спроси и обнови документацию. Открытых вопросов сейчас нет (AGENTS §15).
- Перед вопросами об архитектуре читай [`graphify-out/GRAPH_REPORT.md`](graphify-out/GRAPH_REPORT.md); после изменений кода обновляй граф: `graphify update .` (симлинк в `~/.local/bin/graphify`).
