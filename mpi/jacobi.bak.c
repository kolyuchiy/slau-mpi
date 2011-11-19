
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "util2.h"

#define ROOT (0)

void jacobi_iter(double *A, double *x, double *x_old, int n_part, int n, int first)
{
	int i, j;
	double sum;

	for (i=0; i<n_part; i++) {
		sum = 0;
		for (j=0; j<i+first; j++) {
			sum += A[i*(n+1)+j] * x_old[j];
		}

		for (j=i+first+1; j<n; j++) {
			sum += A[i*(n+1)+j] * x_old[j];
		}

		x[i+first] = (A[i*(n+1)+n] - sum) / A[i*(n+1)+i+first];
	}
}

unsigned jacobi_solve(double *A, double *x, double e, int n, int myid, int n_part, int numprocs)
{
	double *x_old;
	int i, iter = 0, first;
	double d_norm, d_val;
	int *sendcnts, *displs;

	displs = (int*)malloc(numprocs*sizeof(int));
	sendcnts = (int*)malloc(numprocs*sizeof(int));

	MPI_Scan(&n_part, &first, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	first -= n_part;

	MPI_Allgather(&n_part, 1, MPI_INT, sendcnts, 1, MPI_INT, MPI_COMM_WORLD);
	displs[0] = 0;
	for (i=1; i<numprocs; i++) displs[i] = displs[i-1] + sendcnts[i-1];

	x_old = new_vector(n);

	do {
		iter++;

		memcpy(x_old, x, sizeof(double)*n);

		jacobi_iter(A, x, x_old, n_part, n, first);

// TODO		Gatherv?
		MPI_Allgatherv(x+first, n_part, MPI_DOUBLE, x, sendcnts, displs, MPI_DOUBLE, MPI_COMM_WORLD);

		d_norm = 0;
		if (myid == ROOT) {
			for (i=0; i<n; i++) {
				d_val = fabs(x[i] - x_old[i]);
				if (d_norm < d_val) d_norm = d_val;
			}
		}

		MPI_Bcast(&d_norm, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
	} while (e < d_norm);

	free(x_old);

	return iter;
}

int main(int argc, char **argv)
{
	double *A, *A_part, *x;
	int i, n, n_part, part_size, iter, myid, numprocs;
	double error = 0.1e-10;
	int *sendcnts, *displs;
	double t_start, t_end;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);

	printf("process %i on %s\n", myid, processor_name);
	
	scanf("%u", &n);
	MPI_Bcast(&n, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
	MPI_Bcast(&error, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	if (myid == ROOT) {
		t_start = MPI_Wtime();
		A = new_matrix(n, n+1);
		read_matrix(A, n, n+1);
		t_end = MPI_Wtime();
		printf("Time to input matrix: %f sec\n", t_end - t_start);
	}

	x = new_vector(n);
	MPI_Bcast(x, n, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	n_part = (n/numprocs) + (n%numprocs > myid ? 1 : 0);
//	printf("process: %i; num of rows: %i\n", myid, n_part);

	A_part = new_matrix(n_part, n+1);

	displs = (int*)malloc(numprocs*sizeof(int));
	sendcnts = (int*)malloc(numprocs*sizeof(int));

	part_size = (n+1) * n_part;
	MPI_Gather(&part_size, 1, MPI_INT, sendcnts, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

	displs[0] = 0;
	for (i=1; i<numprocs; i++) displs[i] = displs[i-1] + sendcnts[i-1];

/*
if (myid == ROOT) { 
	printf("#%i, %p: ", myid, A_part); write_matrix(A_part, n_part, n+1);
//	printf("part_size #%i - %i\n", myid, part_size);
	printf("sendcnts #%i: ", myid); for (i=0; i<numprocs; i++) printf("%i ", sendcnts[i]); printf("\n");
	printf("displs #%i: ", myid); for (i=0; i<numprocs; i++) printf("%i ", displs[i]); printf("\n");
	printf("A #%i: ", myid); write_matrix(A, n, n+1); 
}
*/

	t_start = MPI_Wtime();
	MPI_Scatterv(A, sendcnts, displs, MPI_DOUBLE, A_part, part_size, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
//	printf("#%i: ", myid); write_matrix(A_part, n_part, n+1);
	t_end = MPI_Wtime();
	if (myid == ROOT) printf("Time to scatter matrix to all processors: %f sec\n", t_end - t_start);

	t_start = MPI_Wtime();
	iter = jacobi_solve(A_part, x, error, n, myid, n_part, numprocs);
	t_end = MPI_Wtime();
	if (myid == ROOT) printf("Time to solve equation: %f sec\n", t_end - t_start);

	if (myid == ROOT) {
		printf("iter %i\n", iter);
		printf("ans = \n");
		write_vector(x, n); 
	}

	if (myid == ROOT) {
		free_matrix(A);
	}
	free(x);
	free_matrix(A_part);
	free(sendcnts);
	free(displs);

	MPI_Finalize();

	return EXIT_SUCCESS;
}

