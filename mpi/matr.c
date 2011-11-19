
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"

int my_id, numprocs, Root = 0, *sendcounts, *displs;

void jacobi_mpi_iter(double **A, double *b, double *x, double *x_old, int n, int matr_size, int first)
{
	int i, j;
	double sum;

	for (i=0; i<n; i++) {
		sum = 0;
		for (j=0; j<i+first; j++) {
			sum += A[i][j] * x_old[j];
		}

		for (j=i+first+1; j<matr_size; j++) {
			sum += A[i][j] * x_old[j];
		}

		x[i+first] = (b[i] - sum) / A[i][i+first];
	}
}

unsigned jacobi_mpi_solve(double **A, double *b, double *x, double e, int n, int matr_size)
{
	double *x_old;
	int i, iter = 0, result, first;
	double d_norm = 0, d_val;

	MPI_Scan(&n, &first, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	first -= n;

	MPI_Allgather(&n, 1, MPI_INT, sendcounts, 1, MPI_INT, MPI_COMM_WORLD);

	displs[0] = 0;
	for (i=1; i<n; i++) {
		displs[i] = displs[i-1] + sendcounts[i-1];
	}

	x_old = new_vector(n);

	do {
		iter++;

		memcpy(x_old, x, sizeof(double)*matr_size);

		jacobi_mpi_iter(A, b, x, x_old, n, matr_size, first);

		MPI_Allgatherv(&x[first], n, MPI_DOUBLE, x, sendcounts, displs, MPI_DOUBLE, MPI_COMM_WORLD);

		if (my_id == Root) {
			d_norm = 0;
			for (i=0; i<n; i++) {
				d_val = fabs(x[i] - x_old[i]);
				if (d_norm < d_val) d_norm = d_val;
			}
			result = e < d_norm;
		}

		MPI_Bcast(&result, 1, MPI_INT, Root, MPI_COMM_WORLD);
	} while (result);

	free(x_old);

	return iter;
}

int main(int argc, char **argv)
{
	double **A2, *b2, **A, *b, *x;
	int n, iter, size, size2, endcounts;
	double error = 0.1e-10;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	scanf("%u", &n);

	if (my_id == Root) {
		A2 = new_matrix(n,n);
		b2 = new_vector(n);
	}

	MPI_Bcast(&n, 1, MPI_INT, Root, MPI_COMM_WORLD);
	MPI_Bcast(&error, 1, MPI_DOUBLE, Root, MPI_COMM_WORLD);

	b = new_vector(n);
	A = new_matrix(n, n);
	x = new_vector(n);

	if (my_id = Root) {
	}

	MPI_Bcast(x, n, MPI_DOUBLE, Root, MPI_COMM_WORLD);
	size = (n/numprocs) + (n%numprocs > my_id ? 1 : 0);
	displs = (int*)malloc(numprocs*sizeof(int));
	sendcounts = (int*)malloc(numprocs*sizeof(int));

	size2 = n * size;
	MPI_Gather(&size2, 1, MPI_INT, sendcounts, 1, MPI_INT, Root, MPI_COMM_WORLD);

	read_vector(b, n);
	read_matrix(A, n, n);

	iter = jacobi_mpi_solve(A, b, x, error, size, n);

	printf("iter %i\n", iter);
	printf("ans = \n");
	write_vector(x, n);

	free(b);
	free_matrix(A, n);
	free(x);

	return EXIT_SUCCESS;
}

