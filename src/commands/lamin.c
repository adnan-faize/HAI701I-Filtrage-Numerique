#include <stdint.h>
#include <stdlib.h>

#include "utils/note.h"
#include "utils/wave.h"

#include "commands/lamin.h"

#define NOTES_NB 3

int run_lamin_command(const struct config* cfg) {
    if (cfg == NULL) { return EXIT_FAILURE; }

    size_t data_nb = cfg->sampling_freq * cfg->channels_nb * cfg->millis / 1000;

    const double frequencies[NOTES_NB] = {
        440.00, // La (A4)
        523.25, // Do (C5)
        659.25, // Mi (E5)
    };

    uint8_t* data = create_note_u8(
        malloc(data_nb * sizeof(uint8_t)),
        data_nb,
        frequencies,
        NOTES_NB,
        cfg->sampling_freq
    );

    wave* wav = create_wave_u8(
        malloc(sizeof(wave)),
        data,
        data_nb,
        cfg->channels_nb,
        cfg->sampling_freq
    );

    wave_write(wav, "out/lamin.wav");

    analyze_signal_dft(data, data_nb, cfg->sampling_freq);

    destroy_wave(wav);
    return EXIT_SUCCESS;
}
