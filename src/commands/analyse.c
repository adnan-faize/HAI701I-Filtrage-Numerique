#include <stdlib.h>
#include <stdint.h>

#include "utils/note.h"
#include "utils/wave.h"

#include "commands/analyse.h"

// pour la gamme de piano : sol = 349.2. essayer de l'enlever avec fft
// k = (f_c / f_e) * 2^m  (k = (349.2/f_e) * n) (k * f_e / n = 349.2)

int run_analyse_command(const struct config* cfg) {
    wave* wav = create_wave(malloc(sizeof(wave)));
    wave_read(wav, "res/GammePiano.wav");
    analyze_signal_dft(wav->data_8, wav->data_nb, wav->sampling_freq);
    wave_write(wav, "out/piano_modif.wav");
    destroy_wave(wav);
    return EXIT_SUCCESS;
}
