import matplotlib.pyplot as plt
# import pandas as pd 
import numpy as np 
file1 = 'spectrum_a.txt'
file2 = 'spectrum_i.txt'
file3 = 'spectrum_u.txt'
file4 = 'spectrum_e.txt'
file5 = 'spectrum_o.txt'

data1x, data1y = np.loadtxt(file1, skiprows=1,unpack=True)
data2x, data2y = np.loadtxt(file2, skiprows=1,unpack=True)
data3x, data3y = np.loadtxt(file3, skiprows=1,unpack=True)
data4x, data4y = np.loadtxt(file4, skiprows=1,unpack=True)
data5x, data5y = np.loadtxt(file5, skiprows=1,unpack=True)
# print(data1x)
fig = plt.figure(figsize=(6.4,3.2), dpi=300)
ax1 = fig.add_subplot(111)

ax1.plot(data1x, data1y, linewidth=0.5, label='a')
ax1.plot(data2x, data2y, linewidth=0.5, label='i')
ax1.plot(data3x, data3y, linewidth=0.5, label='u')
ax1.plot(data4x, data4y, linewidth=0.5, label='e')
ax1.plot(data5x, data5y, linewidth=0.5, label='o')

ax1.set_xlim([100, 1500])

plt.legend()
plt.show()