
#include <stdlib.h>
#include "util.h"

int main()
{
	double **A, *x;
	int n;
	int i, j, k;

	scanf("%u", &n);

	A = new_matrix(n, n+1);
	x = new_vector(n);

	read_matrix(A, n, n+1);

	gauss_l_factor(A, n);
	gauss_solve_l(A, x, n);

	printf("ans = \n");
	write_vector(x, n);

	free_matrix(A, n);
	free(x);

	return EXIT_SUCCESS;
}

