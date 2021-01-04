// FFT Implementation with uncertainty propagation
// Input must be a power of 2 in length and known


#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "arithmetic_propagation.h"

double PI = atan2(1,1) * 4;

void _fft(complex_uncertain_value buffer[], complex_uncertain_value output[], int n, int step)
{
	if (step < n) {
		_fft(output, buffer, n, step * 2);
		_fft(output + step, buffer + step, n, step * 2);

		for (int i = 0; i < n; i += 2 * step) {
			complex_uncertain_value exponential = {cexp(-I * PI * i / n), +0, +0};
			complex_uncertain_value t = cmult_prop(exponential, output[i + step]);
			buffer[i / 2] = cadd_prop(output[i], t);
			buffer[(i + n) / 2] = csub_prop(output[i], t);
		}
	}
}

void fft(complex_uncertain_value buffer[], int n)
{
	complex_uncertain_value out[n];

	for (int i = 0; i < n; i++) out[i] = buffer[i];

	_fft(buffer, out, n, 1);
}
