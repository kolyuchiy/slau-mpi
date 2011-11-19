
CC          = /usr/local/mpich/bin/mpicc
CLINKER     = /usr/local/mpich/bin/mpicc
SHELL       = /bin/sh
CFLAGS      = -I/usr/local/mpich/include -I..

LIBS =
FLIBS =
EXECS = simple matr jacobi gauss_mpi

all: $(EXECS)

jacobi:	../util2.o jacobi.o
matr: ../util.o matr.o
test1: ../util2.o test1.o
gauss_mpi: ../util2.o gauss_mpi.o

clean:
	-/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $<

