import matplotlib.pyplot as plt
# import pandas as pd 
import numpy as np 
file1 = '../judge.txt'
file2 = '../o.txt'

data1f, data1y = np.loadtxt(file1, unpack=True)
data2f, data2y = np.loadtxt(file2, unpack=True)

fig = plt.figure(figsize=(6.4, 3.2), dpi=300)
ax1 = fig.add_subplot(111)

ax1.scatter(data1f, data1y, s=0.2)
ax1.scatter(data2f, data2y, s=0.2)
ax1.set_xlim([100, 1000])
ax1.set_xticks(np.arange(100, 1000, 50))
ax1.minorticks_on()
ax1.grid(axis='both')
ax1.tick_params(labelsize=5)
plt.savefig('o.png')
plt.show()