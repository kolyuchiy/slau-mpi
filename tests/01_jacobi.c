
#include <stdlib.h>
#include "util.h"
#include "jacobi.h"

int main()
{
	double **A, *x;
	int n, iter;

	scanf("%u", &n);

	A = new_matrix(n, n+1);
	x = new_vector(n);

	read_matrix(A, n, n+1);

	iter = jacobi_solve(A, x, 0.1e-10, n);

	printf("iter %i\n", iter);
	printf("ans = \n");
	write_vector(x, n);

	free_matrix(A, n);
	free(x);

	return EXIT_SUCCESS;
}

