#pragma once

#include <stdlib.h>
#include <stdint.h>

struct config {
    int32_t sampling_freq;
    int16_t channels_nb;
    size_t millis;
    char* output_dir;
};
