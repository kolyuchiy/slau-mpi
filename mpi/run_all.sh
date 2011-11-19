#!/bin/sh

for j in 1 2 3 4 5
do
	for i in 1 2 3 4
	do
		echo `date` ": run on $i processors ${j}000x${j}000 matrix"
		echo "./run_4_tests.sh $i ../matr_${j}000.in"
		./run_4_tests.sh $i ../matr_${j}000.in
	done
done

