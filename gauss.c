
#include "gauss.h"

void gauss_l_factor(double **A, int n) 
{
	int i, j, k;

	for (i=0; i<n; i++) {
		for (k=i+1; k<n; k++) {
			double a = A[k][i] / A[i][i];
			for (j=i; j<n+1; j++)
				A[k][j] -= A[i][j] * a;
		}
	}
}

void gauss_solve_l(double **A, double *x, int n)
{
	int i, j;

	for (i=n-1; i>=0; i--) {
		x[i] = A[i][n];
		for (j=n-1; j>i; j--) {
			x[i] -= x[j] * A[i][j];
		}
		x[i] /= A[i][i];
	}

}

