#pragma once

#include <stdlib.h>
#include <stdint.h>

extern uint8_t* create_note_u8(uint8_t* buffer, const size_t buffer_len, const double* frequencies, const size_t nb_freq, const int16_t sampling_freq);
extern int16_t* create_note_i16(int16_t* buffer, size_t buflen, double freq, int16_t sampling_freq);

extern int analyze_signal_dft(const uint8_t* samples_u8, size_t N, int16_t sampling_freq);
