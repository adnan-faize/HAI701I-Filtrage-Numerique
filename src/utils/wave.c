#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

#include "utils/wave.h"

/**
 * Checks that type sizes are correct
 */
static void check_types_size() {
    if (sizeof(uint8_t) != 1) { exit(EXIT_FAILURE); }
    if (sizeof(int16_t) != 2) { exit(EXIT_FAILURE); }
    if (sizeof(int32_t) != 4) { exit(EXIT_FAILURE); }
}

/**
 *
 */
void init_descriptor(wave* wav, int32_t depth, int16_t channels_nb, int32_t sampling_freq) {
    wav->depth = depth;
    memcpy(wav->file_type, "RIFF", 4);
    memcpy(wav->file_id  , "WAVE", 4);
    memcpy(wav->chunk_id , "fmt ", 4);
    memcpy(wav->data_id  , "data", 4);
    wav->data_size = (wav->depth / 8) * wav->data_nb;
    wav->file_size = 44 + wav->data_size - 8;
    wav->chunk_size = 16;
    wav->format = 1;
    wav->channels_nb = channels_nb;
    wav->sampling_freq = sampling_freq;
    wav->bytes_per_sample = wav->channels_nb * wav->depth / 8;
    wav->bytes_per_second = wav->sampling_freq * wav->bytes_per_sample;
}

wave* create_wave(wave* wav) {
    if (wav == NULL) { return NULL; }
    check_types_size();
    wav->is_data_8_allocated = false;  // tableau non alloué
    wav->is_data_16_allocated = false; // tableau non alloué
    return wav;
}

/**
 * Creates a wave struct with the 8bit data passed.
 * if NULL pointer passed for wav, uses malloc to get one.
 * else use pointer given.
 */
wave* create_wave_u8(wave* wav, uint8_t* data_8, size_t data_nb, int16_t channels_nb, int32_t sampling_freq) {
    if (wav == NULL) { return NULL; }
    check_types_size();
    wav->data_nb = data_nb;
    wav->data_8 = data_8;
    wav->is_data_8_allocated = true;
    wav->is_data_16_allocated = false;
    init_descriptor(wav, 8, channels_nb, sampling_freq);
    return wav;
}

wave* create_wave_i16(wave* wav, int16_t* data_16, size_t data_nb, int16_t channels_nb, int32_t sampling_freq) {
    if (wav == NULL) { return NULL; }
    check_types_size();
    wav->data_nb = data_nb;
    wav->data_16 = data_16;
    wav->is_data_8_allocated = false;
    wav->is_data_16_allocated = true;
    init_descriptor(wav, 16, channels_nb, sampling_freq);
    return wav;
}

void destroy_wave(wave* wav) {
    if (wav->is_data_8_allocated) { free(wav->data_8); }
    else if (wav->is_data_16_allocated) { free(wav->data_16); }
    free(wav);
}

// uint8_t** wave_get_data_u8(wave* wav, uint8_t** data8, size_t size) {
//  todo
// }

int wave_write(wave* wav, char* filename) {
    if (wav == NULL) { return EXIT_FAILURE; }

    FILE* fd = fopen(filename, "wb");
    if (fd == NULL) { return EXIT_FAILURE; }

    uint8_t header[44];

    size_t pos = 0;
    size_t i;

    memcpy(&header[pos], wav->file_type, 4); pos += 4;
    for (i = 0; i < 4; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->file_size) >> (8 * i); }
    memcpy(&header[pos], wav->file_id, 4); pos += 4;
    memcpy(&header[pos], wav->chunk_id, 4); pos += 4;
    for (i = 0; i < 4; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->chunk_size) >> (8 * i); }
    for (i = 0; i < 2; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->format) >> (8 * i); }
    for (i = 0; i < 2; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->channels_nb) >> (8 * i); }
    for (i = 0; i < 4; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->sampling_freq) >> (8 * i); }
    for (i = 0; i < 4; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->bytes_per_second) >> (8 * i); }
    for (i = 0; i < 2; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->bytes_per_sample) >> (8 * i); }
    for (i = 0; i < 2; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->depth) >> (8 * i); }
    memcpy(&header[pos], wav->data_id, 4); pos += 4;
    for (i = 0; i < 4; i++, pos++) { header[pos] = ((255 << (8 * i)) & wav->data_size) >> (8 * i); }

    if (fwrite(header, 1, 44, fd) != 44) {
        fclose(fd);
        return EXIT_FAILURE;
    }

    switch (wav->depth) {
        case 8:
            if (fwrite(wav->data_8, 1, wav->data_nb, fd) != wav->data_nb) {
                fclose(fd);
                return EXIT_FAILURE;
            }
            break;
        case 16:
            if (fwrite(wav->data_16, 2, wav->data_nb, fd) != wav->data_nb) {
                fclose(fd);
                return EXIT_FAILURE;
            }
            break;
        default:
            fclose(fd);
            return EXIT_FAILURE;
    }

    printf("Finished writing '%s'.\n", filename);
    fclose(fd);
    return EXIT_SUCCESS;
}

int wave_read(wave* wav, char* filename) {
    if (wav == NULL) { return EXIT_FAILURE; }

    FILE* fd = fopen(filename, "rb");
    if (fd == NULL) { return EXIT_FAILURE; }

    uint8_t header[44];

    if (fread(header, 1, 44, fd) != 44) {
        fclose(fd);
        return EXIT_FAILURE;
    }

    size_t pos = 0;
    size_t i;

    memcpy(wav->file_type, &header[pos], 4); pos += 4;
    // if (strcmp(wav->file_type, "RIFF") != 0) { fclose(fd); return EXIT_FAILURE; }
    wav->file_size = 0;
    for (i = 0; i < 4; i++, pos++) { wav->file_size |= (uint32_t)header[pos] << (8 * i); }
    memcpy(wav->file_id, &header[pos], 4); pos += 4;
    // if (strcmp(wav->file_id, "WAVE") != 0) { fclose(fd); return EXIT_FAILURE; }
    memcpy(wav->chunk_id, &header[pos], 4); pos += 4;
    // if (strcmp(wav->chunk_id, "fmt ") != 0) { fclose(fd); return EXIT_FAILURE; }
    wav->chunk_size = 0;
    for (i = 0; i < 4; i++, pos++) { wav->chunk_size |= (uint32_t)header[pos] << (8 * i); }
    wav->format = 0;
    for (i = 0; i < 2; i++, pos++) { wav->format |= (uint32_t)header[pos] << (8 * i); }
    wav->channels_nb = 0;
    for (i = 0; i < 4; i++, pos++) { wav->channels_nb |= (uint32_t)header[pos] << (8 * i); }
    wav->sampling_freq = 0;
    for (i = 0; i < 4; i++, pos++) { wav->sampling_freq |= (uint32_t)header[pos] << (8 * i); }
    wav->bytes_per_second = 0;
    for (i = 0; i < 4; i++, pos++) { wav->bytes_per_second |= (uint32_t)header[pos] << (8 * i); }
    wav->bytes_per_sample = 0;
    for (i = 0; i < 4; i++, pos++) { wav->bytes_per_sample |= (uint32_t)header[pos] << (8 * i); }
    // if (wav->bytes_per_sample != wav->bytes_per_second / wav->sampling_freq) { fclose(fd); return EXIT_FAILURE; }
    uint16_t depth = 0;
    for (i = 0; i < 2; i++, pos++) { depth |= (uint16_t)header[pos] << (8 * i); }
    wav->depth = depth;
    printf("%d\n", wav->depth);
    // if (wav->depth != 8 * wav->bytes_per_sample / wav->channels_nb) { fclose(fd); return EXIT_FAILURE; }
    memcpy(wav->data_id, &header[pos], 4); pos += 4;
    // if (strcmp(wav->data_id, "data") != 0) { fclose(fd); return EXIT_FAILURE; }
    wav->data_size = 0;
    for (i = 0; i < 4; i++, pos++) { wav->data_size |= (int32_t)header[pos] << (8 * i); }
    // if (wav->data_size != wav->file_size - 36) { fclose(fd); return EXIT_FAILURE; }

    switch (wav->depth) {
        case 8:
            wav->data_nb = wav->data_size;
            wav->data_8 = malloc(wav->data_nb * sizeof(uint8_t));
            if (fread(wav->data_8, 1, wav->data_nb, fd) != wav->data_nb) {
                fclose(fd);
                return EXIT_FAILURE;
            }
            wav->is_data_8_allocated = true;
            wav->is_data_16_allocated = false;
            break;
        case 16:
            wav->data_nb = wav->data_size / 2;
            wav->data_16 = malloc(wav->data_nb * sizeof(int16_t));
            if (fread(wav->data_16, 1, wav->data_nb, fd) != wav->data_nb) {
                fclose(fd);
                return EXIT_FAILURE;
            }
            wav->is_data_8_allocated = false;
            wav->is_data_16_allocated = true;
            break;
        default:
            return EXIT_FAILURE;
    }

    printf("Finished reading '%s'.\n", filename);
    fclose(fd);
    return EXIT_SUCCESS;
}
