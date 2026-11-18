#include "utils/note.h"
#include "utils/wave.h"
#include <stdlib.h>

#include "commands/la.h"

int run_la_command(const struct config* cfg) {
    size_t data_nb = cfg->sampling_freq * cfg->channels_nb * (cfg->millis / 1000);
    wave* wav = create_wave_u8(
        NULL,
        create_note_u8(
            NULL,
            data_nb,
            440,
            cfg->sampling_freq
        ),
        data_nb,
        cfg->channels_nb,
        cfg->sampling_freq
    );
    wave_write(wav, "out/la.wav");
    return EXIT_SUCCESS;
}
