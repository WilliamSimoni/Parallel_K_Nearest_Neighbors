CC = g++

# compiler flags:
#  -o3    - vectorization optimization
CFLAGS  = -O3 -std=c++17 -ftree-vectorizer-verbose=2

# The build target 
TARGETS = Sequential Parallel FastflowPar PointVector IOUtility utimer

all: $(TARGETS) 

Sequential: Sequential.cpp IOUtility utimer
	$(CC) $(CFLAGS) $< -o $@ IOUtility PointVector utimer

Parallel: Parallel.cpp IOUtility utimer
	$(CC) $(CFLAGS) $< -o $@ IOUtility PointVector -pthread utimer

FastflowPar: FastflowPar.cpp IOUtility utimer
	$(CC) $(CFLAGS) -lpthread -Ifastflow $< -o $@ IOUtility PointVector -pthread utimer

IOUtility: IOUtility.cpp PointVector
	$(CC) $(CFLAGS) $< -c -o $@ PointVector

PointVector: PointVector.cpp
	$(CC) $(CFLAGS) $< -c -o $@

utimer: utimer.cpp
	$(CC) $(CFLAGS) $< -c -o $@
	
clean:
	$(RM) $(TARGETS) resultSeq.txt resultFastFlow.txt resultParallel.txt