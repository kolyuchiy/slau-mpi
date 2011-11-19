#!/bin/sh

np=$1
stdin=$2

for i in 1 2 3 4 
do
	echo "run #$i"
	echo "/usr/local/bin/mpirun -np $np -stdin $stdin ./jacobi"
	/usr/local/bin/mpirun -np $np -stdin $stdin ./jacobi |grep Time
done


