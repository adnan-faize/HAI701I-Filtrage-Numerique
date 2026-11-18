#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "utils/note.h"


uint8_t* create_note_u8(uint8_t* buffer, size_t buffer_len, double freq, int16_t sampling_freq) {
    if (buffer == NULL) { if ((buffer = malloc(sizeof(buffer_len))) == NULL) { return NULL; } }

    for (size_t i = 0; i < buffer_len; i++) {
        float x_i = sin(2 * M_PI * freq * i / sampling_freq);
        buffer[i] = (x_i + 1) * 255 / 2;
    }

    return buffer;
}

int16_t* create_note_i16(int16_t* buffer, size_t buffer_len, double freq, int16_t sampling_freq) {
    return buffer;
}
