#pragma once

#include <stdlib.h>
#include <stdint.h>

extern uint8_t* create_note_u8(uint8_t* buffer, size_t buflen, double freq, int16_t sampling_freq);
extern int16_t* create_note_i16(int16_t* buffer, size_t buflen, double freq, int16_t sampling_freq);
