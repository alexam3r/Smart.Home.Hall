#pragma once

#include <cstdint>

// 0…100 % → 0…1023 (кривая CIE1931, таблица в PROGMEM).
uint16_t cie1931(uint8_t percent);
