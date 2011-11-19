
#include <mpi.h>
#include <stdlib.h>
#include "util2.h"

#define ROOT (0)

void gauss_l_factor(double *A, int n) 
{
	int i, j, k, l;

	for (k=0; k<n-1; k++) {
		for (l=k+1; l<n; l++) {
			A[l*(n+1)+k] /= A[k*(n+1)+k];
		}

		for (i=k+1; i<n; i++) {
			for (j=k+1; j<n+1; j++) {
				A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
			}
		}
	}
}

void gauss_solve_l(double *A, double *x, int n)
{
	int i, j;

	for (i=n-1; i>=0; i--) {
		x[i] = A[i*(n+1)+n];
		for (j=n-1; j>i; j--) {
			x[i] -= x[j] * A[i*(n+1)+j];
		}
		x[i] /= A[i*(n+1)+i];
	}

}

int main(int argc, char **argv)
{
	double *A, *x;
	int n;
	int i, j, k;
	int numprocs, myid;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if (myid == ROOT) scanf("%u", &n);
	MPI_Bcast(&n, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

	x = new_vector(n);
	MPI_Bcast(x, n, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	A = new_matrix(n, n+1);
	if (myid == ROOT) read_matrix(A, n, n+1);
	MPI_Bcast(A, n*(n+1), MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	gauss_l_factor(A, n);
	gauss_solve_l(A, x, n);

	if (myid == ROOT) {
		printf("ans = \n");
		write_vector(x, n);

		free_matrix(A);
		free(x);
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}

