#!/bin/bash

g++ -pthread -o somador adder.cpp

for n in 1000000000
do
    for k in 1 2 4 8 16 32 64 128 256
    do
        SOMA=0
        for j in $(seq 1 10);
        do
            OUTPUT=$(./somador $k $n)
            let "SOMA=SOMA+OUTPUT"
        done 
        echo "$k $n"
        MEDIA=$((SOMA/10))
        echo $MEDIA
    done
done