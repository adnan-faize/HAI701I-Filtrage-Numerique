#include <stdlib.h>
#include <stdint.h>

#include "utils/note.h"
#include "utils/wave.h"

#include "commands/la.h"

int run_la_command(const struct config* cfg) {
    size_t data_nb = cfg->sampling_freq * cfg->channels_nb * (cfg->millis / 1000);
    double note = 440;
    wave* wav = create_wave_u8(
        malloc(sizeof(wave)),
        create_note_u8(
            malloc(data_nb * sizeof(uint8_t)),
            data_nb,
            &note,
            1,
            cfg->sampling_freq
        ),
        data_nb,
        cfg->channels_nb,
        cfg->sampling_freq
    );
    wave_write(wav, "out/la.wav");
    destroy_wave(wav);
    return EXIT_SUCCESS;
}
