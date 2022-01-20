#!/bin/bash

#Create output file, override if already present  
test_sequential_result=test_sequential_result.txt  

DATA=./data/100000.txt
K=15

TRIALS=5

echo "Parallel KNN tests with K=$K on dataset $DATA
Average time computed in $TRIALS trials
" > $test_sequential_result

#Sequential
echo "---------- SEQUENTIAL ----------
" >> $test_sequential_result

echo "Sequential test" >> $test_sequential_result

for ((i=1;i<=TRIALS;i++)); do
    ./Sequential $DATA $K
done | grep -Eo '[0-9]+' | awk 'NR%2{e1+=$1;c++;next}{o1+=$1;d++}END{print sprintf("non-serial time: %.0f usec\nserial time: %.0f usec\nentire program time: %.0f usec\n%% serial: %.3f %%",e1/c, o1/d - e1/c, o1/d, ((o1/d - e1/c)/(o1/d)) * 100)}' >> $test_sequential_result