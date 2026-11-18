#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    // chunck type
    char file_type[4];
    char file_id[4];
    int32_t file_size;
    // chunk format
    char chunk_id[4];
    int32_t chunk_size;
    int16_t format;
    int16_t channels_nb;
    int32_t sampling_freq;
    int32_t bytes_per_second;
    int16_t bytes_per_sample;
    int16_t depth;
    // chunk data
    char data_id[4];
    int32_t data_size;
    // data
    uint8_t* data_8;
    int16_t* data_16;
    size_t data_nb;
    // flags
    bool is_data_8_allocated;
    bool is_data_16_allocated;
} wave;

extern wave* create_wave(wave* wav);
extern wave* create_wave_u8(wave* wav, uint8_t* data_8, size_t data_nb, int16_t channels_nb, int32_t sampling_freq);
extern wave* create_wave_i16(wave* wav, int16_t* data_16, size_t data_nb, int16_t channels_nb, int32_t sampling_freq);

extern uint8_t** wave_get_data_u8();
extern int16_t** wave_get_data_i16();

extern int wave_write(wave* wav, char* filename);
extern int wave_read(wave* wav, char* filename);
