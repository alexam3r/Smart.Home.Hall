#include "core/Cie1931.h"

#ifdef ARDUINO
#include <pgmspace.h>
#define ARGON_PROGMEM PROGMEM
#define ARGON_READ_WORD(p) pgm_read_word(p)
#else
#define ARGON_PROGMEM
#define ARGON_READ_WORD(p) (*(p))
#endif

#include "core/Cie1931Table.h"

uint16_t cie1931(uint8_t percent) {
    if (percent > 100) {
        percent = 100;
    }
    return static_cast<uint16_t>(ARGON_READ_WORD(&kCieLut[percent]));
}
