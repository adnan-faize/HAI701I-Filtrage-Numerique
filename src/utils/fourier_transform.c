#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "utils/fourier_transform.h"

int direct_fourier_transform(double *signal, double *re, double *im, size_t N) {
    if (signal == NULL || re == NULL || im == NULL) { return EXIT_FAILURE; }

    for (size_t k = 0; k < N; k++) {
        double real = 0.0;
        double imaginary = 0.0;

        for (size_t n = 0; n < N; n++) {
            double angle = 2.0 * M_PI * (double)k * (double)n / (double)N;

            real += signal[n] * cos(angle);
            imaginary -= signal[n] * sin(angle);
        }

        re[k] = real;
        im[k] = imaginary;
    }

    return EXIT_SUCCESS;
}

int inverse_direct_fourier_transform(double *signal, double *re, double *im, size_t N) {
    if (signal == NULL || re == NULL || im == NULL) { return EXIT_FAILURE; }

    for (size_t n = 0; n < N; n++) {
        double sum = 0.0;

        for (size_t k = 0; k < N; k++) {
            double angle = 2.0 * M_PI * (double)k * (double)n / (double)N;

            sum += re[k] * cos(angle) - im[k] * sin(angle);
        }

        signal[n] = sum / (double)N;
    }

    return EXIT_SUCCESS;
}

int fast_fourier_transform1(double* re, double* im, size_t M, uint8_t flag) {
    if (re == NULL || im == NULL) { return EXIT_FAILURE; }

    return EXIT_SUCCESS;
}

int fast_fourier_transform(int dir, int m, double* x, double* y) {
    int n,i,i1,j,k,i2,l,l1,l2;
	double c1,c2,tx,ty,t1,t2,u1,u2,z;

	/* Calculate the number of points */
	n = 1;
	for (i=0;i<m;i++)
		n *= 2;

	/* Do the bit reversal */
	i2 = n >> 1;
	j = 0;
	for (i=0;i<n-1;i++) {
		if (i < j) {
			tx = x[i];
			ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
		}
		k = i2;
		while (k <= j) {
			j -= k;
			k >>= 1;
		}
		j += k;
	}

	/* Compute the FFT */
	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;
	for (l=0;l<m;l++) {
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;
		for (j=0;j<l1;j++) {
			for (i=j;i<n;i+=l2) {
				i1 = i + l1;
				t1 = u1 * x[i1] - u2 * y[i1];
				t2 = u1 * y[i1] + u2 * x[i1];
				x[i1] = x[i] - t1;
				y[i1] = y[i] - t2;
				x[i] += t1;
				y[i] += t2;
			}
			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		c2 = sqrt((1.0 - c1) / 2.0);
		if (dir == 1)
			c2 = -c2;
		c1 = sqrt((1.0 + c1) / 2.0);
	}

	/* Scaling for forward transform */
	if (dir == 1) {
		for (i=0;i<n;i++) {
			x[i] /= n;
			y[i] /= n;
		}
	}

    return EXIT_SUCCESS;
}
