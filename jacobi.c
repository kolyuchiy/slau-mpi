
#include <math.h>
#include "util.h"
#include "jacobi.h"

void jacobi_iter(double **A, double *x, double *x_old, int n)
{
	int i, j;
	double sum;

	for (i=0; i<n; i++) {
		sum = 0;
		for (j=0; j<i; j++) {
			sum += A[i][j] * x_old[j];
		}

		for (j=i+1; j<n; j++) {
			sum += A[i][j] * x_old[j];
		}

		x[i] = (A[i][n] - sum) / A[i][i];
	}
}

unsigned jacobi_solve(double **A, double *x, double e, int n)
{
	double *x_old;
	int i, iter = 0;
	double d_norm, d_val;

	x_old = new_vector(n);

	do {
		iter++;

		memcpy(x_old, x, sizeof(double)*n);

		jacobi_iter(A, x, x_old, n);
		d_norm = 0;
		for (i=0; i<n; i++) {
			d_val = fabs(x[i] - x_old[i]);
			if (d_norm < d_val) d_norm = d_val;
		}

		if (iter % 10 == 0) printf("10 iterations done\n");
	} while (e < d_norm);

	free(x_old);

	return iter;
}

