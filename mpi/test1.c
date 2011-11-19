
#include <stdlib.h>
#include <mpi.h>
#include "util2.h"

#define ROOT (0)

int main(int argc, char **argv) 
{
	double *A, *x, *A_part;
	int *sendcnts, *displs;
	int numprocs, myid, n, n_part, i;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	n = 2;
	if (myid == ROOT) {
		A = new_matrix(n, n+1);
		A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 4; A[4] = 5; A[5] = 6; 
//		write_matrix(A, n, n+1);
	}
//printf("A %p #%i: ", A, myid); for (i=0; i<8; i++) printf("%f ", A[i]); printf("\n");

	n_part = (8/numprocs) + (8%numprocs > myid ? 1 : 0);
	printf("process: %i; num of rows: %i\n", myid, n_part);

	A_part = (double*)malloc(n_part*sizeof(double));

	displs = (int*)malloc(numprocs*sizeof(int));
	sendcnts = (int*)malloc(numprocs*sizeof(int));

	MPI_Gather(&n_part, 1, MPI_INT, sendcnts, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

	displs[0] = 0;
	for (i=1; i<numprocs; i++) displs[i] = displs[i-1] + sendcnts[i-1];

displs[0] = 0;
displs[1] = 1;
displs[2] = 0;

printf("sendcnts: "); for (i=0; i<numprocs; i++) printf("%i ", sendcnts[i]); printf("\n");
printf("displs: "); for (i=0; i<numprocs; i++) printf("%i ", displs[i]); printf("\n");

printf("A #%i: ", myid); for (i=0; i<8; i++) printf("%f ", A[i]); printf("\n");

	MPI_Scatterv(A, sendcnts, displs, MPI_DOUBLE, A_part, n_part, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
	printf("#%i\n", myid);

printf("A_part %p, #%i: ", A_part, myid); for (i=0; i<n_part; i++) printf("%f ", A_part[i]); printf("\n");

	free(sendcnts);
	free(displs);

	MPI_Finalize();

	return EXIT_SUCCESS;
}

