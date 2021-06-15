#!/bin/bash

#Create output file, override if already present  
test_result=test_result.txt  

DATA=./data/10000.txt
K=3

TRIALS=2

MAX_N_W=4
INIT_N_W=1
STEP_N_W=1


echo "Parallel KNN tests with K=$K on dataset $DATA
Average time computed in $TRIALS trials
" > test_result

#Sequential
echo "---------- SEQUENTIAL ----------
" >> test_result

echo "Sequential test" >> test_result

for ((i=1;i<=TRIALS;i++)); do
    ./Sequential $DATA $K
done | grep -Eo '[0-9]+' | awk 'NR%2{e1+=$1;c++;next}{o1+=$1;d++}END{print sprintf("non-serial time: %.0f usec\nserial time: %.0f usec\nentire program time: %.0f usec\n%% serial: %.3f %%",e1/c, o1/d - e1/c, o1/d, (o1/d - e1/c)/o1/d * 100)}' >> test_result

#Parallel C++
echo "
---------- PARALLEL WITH C++ THREADS ----------" >> test_result

for ((nw=INIT_N_W;nw<=MAX_N_W;nw=STEP_N_W+nw)); do
  echo "
Number of Workers = $nw" >> test_result
  for ((i=1;i<=TRIALS;i++)); do
    ./Parallel $DATA $K $nw
  done | grep -Eo '[0-9]+' | awk 'NR%2{e1+=$1;c++;next}{o1+=$1;d++}END{print sprintf("non-serial time: %.0f usec\nserial time: %.0f usec\nentire program time: %.0f usec\n%% serial: %.3f %%",e1/c, o1/d - e1/c, o1/d, (o1/d - e1/c)/o1/d * 100)}' >> test_result
done

#Tensorflow
echo "
---------- PARALLEL WITH FASTFLOW ----------" >> test_result

for ((nw=INIT_N_W;nw<=MAX_N_W;nw=STEP_N_W+nw)); do
  echo "
Number of Workers = $nw" >> test_result
  for ((i=1;i<=TRIALS;i++)); do
    ./FastflowPar $DATA $K $nw
  done | grep -Eo '[0-9]+' | awk 'NR%2{e1+=$1;c++;next}{o1+=$1;d++}END{print sprintf("non-serial time: %.0f usec\nserial time: %.0f usec\nentire program time: %.0f usec\n%% serial: %.3f %%",e1/c, o1/d - e1/c, o1/d, (o1/d - e1/c)/o1/d * 100)}' >> test_result
done
