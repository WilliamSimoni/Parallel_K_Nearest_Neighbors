CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
#  -03    - vectorization optimization
CFLAGS  = -o3 -std=c++17

# The build target 
TARGETS = Sequential PointVector IOUtility

all: $(TARGETS) 

Sequential: Sequential.cpp PointVector IOUtility
	$(CC) $(CFLAGS) $< -o $@ IOUtility PointVector

IOUtility: IOUtility.cpp PointVector
	$(CC) $(CFLAGS) $< -c -o $@ PointVector

PointVector: PointVector.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	$(RM) $(TARGET)