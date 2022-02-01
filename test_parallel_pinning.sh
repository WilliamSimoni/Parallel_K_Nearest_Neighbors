#!/bin/bash

#Create output file, override if already present  
test_result=test_parallel_pinning.txt  #where we save the result

DATA=./data/100000.txt  #dataset for the test
K=15                    #parameter k of Kmenas

TRIALS=5                #number of trials for each test. The final time will be the mean over all the trials.

MAX_N_W=255             #maximum number of workers to be considerd
INIT_N_W=1              #minimum number of workers to be considerd
STEP_N_W=2              #for example, using step = 2, we will try a number of workers which is 1, 3, 5, 7,


echo "Parallel KNN tests with K=$K on dataset $DATA
Average time computed in $TRIALS trials
" > $test_result

#Parallel C++ with pinning

echo "
---------- PARALLEL WITH PINNING ----------" >> $test_result

for ((nw=INIT_N_W;nw<=MAX_N_W;nw=STEP_N_W+nw)); do
  echo "
Number of Workers = $nw" >> $test_result
  for ((i=1;i<=TRIALS;i++)); do
    ./ParallelPinned $DATA $K $nw
  done | grep -Eo '[0-9]+' | awk 'NR%2{e1+=$1;c++;next}{o1+=$1;d++}END{print sprintf("parallel time: %.0f usec\nserial time: %.0f usec\nentire program time: %.0f usec\n%% serial: %.3f %%",e1/c, o1/d - e1/c, o1/d, ((o1/d - e1/c)/(o1/d)) * 100)}' >> $test_result
done