#!/bin/bash
for ((i=0;i<=8;i++))
do
    n=$((2**i))
    ./project1 $n
done

