
#include <stdlib.h>
#include "util.h"

int main()
{
	double **A, *b;
	int n, m;

	scanf("%u", &n);
	scanf("%u", &m);

	b = new_vector(n);
	A = new_matrix(n, m);

	read_vector(b, n);
	write_vector(b, n);

	read_matrix(A, n, m);
	write_matrix(A, n, m);

	printf("%f\n", A[0][1]);

	free(b);
	free_matrix(A, n);

	return EXIT_SUCCESS;
}

