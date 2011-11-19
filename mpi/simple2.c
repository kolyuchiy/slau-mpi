
#include <stdlib.h>
#include <mpi.h>

#define ROOT (0)

int main(int argc, char **argv) 
{
	int myid;
	int n;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if (myid == ROOT) {
		scanf("%i", &n);
	}

	MPI_Bcast(&n, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	printf("%i - %i\n", myid, n);

	MPI_Finalize();

	return EXIT_SUCCESS;
}

