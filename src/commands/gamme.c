#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils/note.h"
#include "utils/wave.h"
#include "shared/config.h"

int run_gamme_command(const struct config *cfg) {
    if (cfg == NULL || cfg->sampling_freq <= 0 || cfg->channels_nb <= 0) {
        return EXIT_FAILURE;
    }

    const int num_notes = 7;
    const double freqs[7] = {
        440.00, // La (A4)
        493.88, // Si (B4)
        523.25, // Do (C5)
        587.33, // Ré (D5)
        659.25, // Mi (E5)
        698.46, // Fa (F5)
        783.99  // Sol (G5)
    };

    // Example: 500 ms per note (or split cfg->millis across the 7 notes)
    size_t samples_per_note = (size_t)cfg->sampling_freq * (size_t)cfg->channels_nb * 500 / 1000;
    size_t total_samples = samples_per_note * (size_t)num_notes;

    uint8_t *scale_buf = malloc(total_samples * sizeof(uint8_t));
    if (scale_buf == NULL) {
        return EXIT_FAILURE;
    }

    for (int n = 0; n < num_notes; n++) {
        uint8_t *note_slice = &scale_buf[(size_t)n * samples_per_note];
        if (create_note_u8(note_slice, samples_per_note, &freqs[n], 1, cfg->sampling_freq) == NULL) {
            free(scale_buf);
            return EXIT_FAILURE;
        }
    }

    wave *wav = create_wave_u8(malloc(sizeof(wave)), scale_buf, total_samples, cfg->channels_nb, cfg->sampling_freq);
    if (wav == NULL) {
        free(scale_buf);
        return EXIT_FAILURE;
    }

    int status = wave_write(wav, "out/gamme.wav");
    destroy_wave(wav); // also frees scale_buf since is_data_8_allocated is true
    return status == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
