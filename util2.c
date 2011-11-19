
#include <stdlib.h>
#include "util2.h"

double* new_vector(int n)
{
	return (double *)malloc(sizeof(double)*n);
}

double* new_matrix(int n, int m)
{
	double *a;
	a = (double *)malloc(sizeof(double)*n*m);
	return a;
}

void free_matrix(double *a) 
{
	free(a);
}

double* read_vector(double *v, int n)
{
	int i;

	for (i=0; i<n; i++) {
		if (!scanf("%lf", &v[i])) {
			printf("invalid input\n");
			exit(EXIT_FAILURE);
		}
	}

	return v;
}

void write_vector(double *v, int n)
{
	int i;

	for (i=0; i<n; i++) {
		printf("%10f ", v[i]);
	}
	printf("\n");
}

double* read_matrix(double *a, int n, int m)
{
	int j;

	for (j=0; j<n; j++) {
		read_vector(a+j*m, m);
	}

	return a;
}

void write_matrix(double *a, int n, int m)
{
	int j;

	for (j=0; j<n; j++) {
		write_vector(a+j*m, m);
	}
}

