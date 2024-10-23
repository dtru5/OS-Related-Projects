#!/bin/bash
# Loop through powers of 2 from 1 to 256
for ((i=0;i<=8;i++))
do
	n=$((2**i))
	./project2 $n
done
