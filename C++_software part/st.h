#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fftt.h"
#define M_PI 3.1415926


static double gauss(int n, int m);

/* Stockwell transform of the real array data. The len argument is the
number of time points, and it need not be a power of two. The lo and hi
arguments specify the range of frequencies to return. If they are
both zero, they default to lo = 0 and hi = len / 2. The result is
returned in the complex array result, which must be preallocated, with
n rows and len columns, where n is hi - lo + 1. For the default values of
lo and hi, n is len / 2 + 1. */

void st(int len, int lo, int hi, int *data, double *result)
{
	int i, k, n, l2;
	double s, *p;
 int len_temp;

	 double *g;

	 COMPX *h, *H, *G;
	 len_temp=len;
	len=pow(2,(ceil)(log(len)/log(2)));
	/* Check for frequency defaults. */

	if (lo == 0 && hi == 0) {
		hi = len / 2;
	}

	/* Keep the arrays and plans around from last time, since this
	is a very common case. Reallocate them if they change. */
	//	free(h);
	//	free(H);
	//	free(G);
	//	free(g);

		h = (COMPX *)malloc(sizeof(COMPX) * len);
		H = (COMPX *)malloc(sizeof(COMPX) * len);
		G = (COMPX *)malloc(sizeof(COMPX) * len);
		g = (double *)malloc(sizeof(double) * len);
	/* Convert the input to complex. Also compute the mean. */

	s = 0.;
	memset(h, 0, sizeof(COMPX) * len);
	memset(H, 0, sizeof(COMPX) * len);
	for (i = 0; i < len_temp; i++) {
		h[i].real = data[i];
		s += data[i];
	}
	s /= len;

	/* FFT. */

	FFT(h, len);

	/* Hilbert transform. The upper half-circle gets multiplied by
	two, and the lower half-circle gets set to zero.  The real axis
	is left alone. */

	l2 = (len + 1) / 2;
	for (i = 1; i < l2; i++) {
		h[i].real *= 2.;
		h[i].imag *= 2.;
	}
	l2 = len / 2 + 1;
	for (i = l2; i < len; i++) {
		h[i].real = 0.;
		h[i].imag = 0.;
	}

	/* Fill in rows of the result. */

	p = result;

	/* The row for lo == 0 contains the mean. */

	n = lo;
	if (n == 0) {
		for (i = 0; i < len; i++) {
			*p++ = s;
			*p++ = 0.;
		}
		n++;
	}

	/* Subsequent rows contain the inverse FFT of the spectrum
	multiplied with the FFT of scaled gaussians. */

	while (n < hi) {

		/* Scale the FFT of the gaussian. Negative frequencies
		wrap around. */

		g[0] = gauss(n, 0);
		l2 = len / 2 + 1;
		for (i = 1; i < l2; i++) {
			g[i] = g[len - i] = gauss(n, i);
		}

		for (i = 0; i < len; i++) {//i就是β
			s = g[i];
			k = n + i;
			if (k >= len) k -= len;
			G[i].real = h[k].real * s;
			G[i].imag = h[k].imag * s;
		}

		/* Inverse FFT the result to get the next row. */

		IFFT(G, len);//采用函数的傅里叶逆变换等效公式中的积分效果
		for (i = 0; i < len; i++) {
			*p++ = sqrt((G[i].real / len)*(G[i].real / len)+ (G[i].imag / len)*(G[i].imag / len));//*p++ = G[i].real / len;
			//*p++ = G[i].imag / len;
		}

		/* Go to the next row. */

		n++;
	}
}

/* This is the Fourier Transform of a Gaussian. */

static double gauss(int n, int m)
{
	return exp(-2. * M_PI * M_PI * m * m / (n * n));
}

/* Inverse Stockwell transform. */


