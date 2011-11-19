
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) 
{
	int myid;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	printf("%i\n", myid);
	

	MPI_Finalize();

	return EXIT_SUCCESS;
}

