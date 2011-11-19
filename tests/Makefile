
CC=gcc
CFLAGS=-I../

all: 00_read_write 01_jacobi 02_gauss_zeidel 03_gauss

00_read_write: 00_read_write.o ../util.o
01_jacobi: 01_jacobi.o ../util.o ../jacobi.o
02_gauss_zeidel: 02_gauss_zeidel.o ../util.o ../gauss_zeidel.o
03_gauss: 03_gauss.o ../util.o ../gauss.o

test: 00_read_write_test 01_jacobi_test 02_gauss_zeidel_test 03_gauss_test

00_read_write_test: 00_read_write
	@./00_read_write < 00_read_write.input 		\
	|  diff - 00_read_write.output >/dev/null	\
	&& echo -e "*** 00_read_write_test - \tok"	\
	|| echo -e "*** 00_read_write_test - \terror"

01_jacobi_test: 01_jacobi
	@./01_jacobi < 01_jacobi.input 			\
	|  diff - 01_jacobi.output >/dev/null		\
	&& echo -e "*** 01_jacobi_test - \tok"		\
	|| echo -e "*** 01_jacobi_test - \terror"

02_gauss_zeidel_test: 02_gauss_zeidel
	@./02_gauss_zeidel < 02_gauss_zeidel.input	\
	|  diff - 02_gauss_zeidel.output >/dev/null	\
	&& echo -e "*** 02_gauss_zeidel_test - \tok"	\
	|| echo -e "*** 02_gauss_zeidel_test - \terror"

03_gauss_test: 03_gauss
	@./03_gauss < 03_gauss.input	\
	|  diff - 03_gauss.output >/dev/null	\
	&& echo -e "*** 03_gauss_test - \tok"	\
	|| echo -e "*** 03_gauss_test - \terror"

clean:
	rm -f *.o *.core 00_read_write 01_jacobi 02_gauss_zeidel 03_gauss
