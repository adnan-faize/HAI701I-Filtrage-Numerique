#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "utils/fourier_transform.h"

#include "utils/note.h"


uint8_t* create_note_u8(uint8_t* buffer, const size_t buffer_len, const double* frequencies, const size_t nb_freq, const int16_t sampling_freq) {
    if (buffer == NULL || frequencies == NULL) { return NULL; }

    for (size_t i = 0; i < buffer_len; i++) {
        double x_i = 0.0;
        for (size_t n = 0; n < nb_freq; n++) { x_i += sin(2 * M_PI * frequencies[n] * i / sampling_freq); }
        x_i = x_i / (double)nb_freq;
        buffer[i] = (uint8_t)((x_i + 1) * 255 / 2);
    }

    return buffer;
}

int16_t* create_note_i16(int16_t* buffer, size_t buffer_len, double freq, int16_t sampling_freq) {
    return buffer;
}



int analyze_signal_dft(const uint8_t* samples_u8, size_t N, int16_t sampling_freq) {
    if (samples_u8 == NULL || N == 0 || sampling_freq <= 0) {
        return -1;
    }

    int m = (N > 1) ? (int)ceil(log2((double)N)) : 1;
    size_t n = (size_t)1 << m; // actual number of FFT points (2^m)

    double *re = calloc(n, sizeof(double));
    double *im = calloc(n, sizeof(double));
    if (re == NULL || im == NULL) {
        free(re);
        free(im);
        return -1;
    }

    // Load N samples into real part; remaining bins [N .. n) stay 0.0
    for (size_t idx = 0; idx < N; idx++) {
        re[idx] = ((double)samples_u8[idx] - 128.0) / 128.0;
    }

    if (fast_fourier_transform(1, m, re, im) != 0) {
        free(re);
        free(im);
        return -1;
    }

    // With dir == 1 scaling by 1/n, a unit sine wave peaks around ~0.5
    for (size_t k = 0; k < n / 2; k++) {
        double freq_hz = (double)k * (double)sampling_freq / (double)n;
        double magnitude = sqrt(re[k] * re[k] + im[k] * im[k]);
        // if (magnitude > 0.05) {
            printf("bin %zu (%.1f Hz): mag = %.4f\n", k, freq_hz, magnitude);
        // }
    }

    free(re);
    free(im);
    return 0;
}
