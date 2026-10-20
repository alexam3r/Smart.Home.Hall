#!/usr/bin/env python3
"""Генерирует include/core/Cie1931Table.h (101 значение, 0…100)."""


def cie(percent: int) -> int:
    if percent <= 0:
        return 0
    if percent >= 100:
        return 1023
    out = percent / 902.3 if percent <= 8 else ((percent + 16.0) / 116.0) ** 3
    return int(out * 1023.0)


def main() -> None:
    print("#pragma once")
    print("#include <cstdint>")
    print()
    print("// Сгенерировано tools/gen_cie_lut.py; не редактировать вручную.")
    print("const uint16_t kCieLut[101] ARGON_PROGMEM = {")
    for start in range(0, 101, 10):
        row = ", ".join(str(cie(p)) for p in range(start, min(start + 10, 101)))
        print(f"    {row},")
    print("};")


if __name__ == "__main__":
    main()
