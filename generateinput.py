import numpy as np
import sys

filename = sys.argv[1]
size = int(sys.argv[2])

f = None

try:
    f = open(filename, "x")
except:
    f = open(filename, "w")

for i in range(0, size):
    f.write(f"{np.random.uniform(low=-120, high=120)},{np.random.uniform(low=-120, high=120)}\n")

f.close()