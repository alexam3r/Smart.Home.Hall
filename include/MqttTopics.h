#pragma once

namespace mqtt {

// Команды (контроллер подписан, retain не используется).
constexpr char LIGHT_CMD[] = "home/hall/light/light";
constexpr char LIGHT_BRIGHTNESS_CMD[] = "home/hall/light/brightness";
constexpr char LIGHT_MOTION_CMD[] = "home/hall/light/moveDetection";
constexpr char FLOOR_CMD[] = "home/hall/floor/floor";
constexpr char FLOOR_MOTION_CMD[] = "home/hall/floor/moveDetection";
constexpr char FLOOR_TARGET[] = "home/hall/floor/targetTemp";

// Состояния.
constexpr char LIGHT_STATE[] = "home/hall/light/light_state";
constexpr char LIGHT_BRIGHTNESS_STATE[] = "home/hall/light/brightness_state";
constexpr char LIGHT_MOTION_STATE[] = "home/hall/light/moveDetection_state";
constexpr char LIGHT_MOVE[] = "home/hall/light/move";
constexpr char FLOOR_STATE[] = "home/hall/floor/floor_state";
constexpr char FLOOR_HEAT_STATE[] = "home/hall/floor/floor_heat_state";
constexpr char FLOOR_MOTION_STATE[] = "home/hall/floor/moveDetection_state";

// Телеметрия.
constexpr char TELEMETRY_STATE[] = "home/hall/state";

}  // namespace mqtt
