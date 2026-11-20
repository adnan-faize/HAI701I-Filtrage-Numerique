#pragma once

#include <stdlib.h>

extern int direct_fourier_transform(double* signal, double* real_part, double* imaginary_part, size_t N);

extern int inverse_direct_fourier_transform(double* signal, double* real_part, double* imaginary_part, size_t N);

extern int fast_fourier_transform(int dir, int m, double* x, double* y);
