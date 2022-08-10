import numpy as np
import matplotlib.pyplot as plt

npe = []
filename = "../build/tmp1.log"
with open(filename) as f:
    for lines in f.readlines():
        line = lines.strip("\n")
        npe.append(float(line))

fig, ax = plt.subplots()
ax.hist(npe, bins=100)
plt.show()
