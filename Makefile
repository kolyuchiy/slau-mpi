
CC=gcc

all:	matr

matr:	matr.o util.o gauss_zeidel.o
gauss_mpi:	gauss_mpi.o util2.o

clean:
	rm -f *.o *.core matr

