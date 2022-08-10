import numpy as np
import matplotlib.pyplot as plt
import ROOT

filename = "./abslength.txt"

photonE, waveL, absL = [], [], []

g1 = ROOT.TGraph()

with open(filename) as f:
    for lines in f.readlines():
        line = lines.strip("\n")
        data = line.split(" ")
        if data[0] == "E":
            photonE.append(float(data[1]))
            waveL.append(1240/float(data[1]))
        if data[0]  == "L":
            absL.append(float(data[1]))

for i in range(len(photonE)):
    g1.SetPoint(i, waveL[i], absL[i])

L430nm = g1.Eval(430.)
print("Absorption @430nm : %.3f"%(L430nm))
print("Scale factor of absorption length : %.3f" %(77./L430nm) )

waveL = np.array(waveL)
absL  = np.array(absL)

fig, ax = plt.subplots()
ax.plot(waveL, absL*77./L430nm, "-")
ax.semilogy()
plt.show()
