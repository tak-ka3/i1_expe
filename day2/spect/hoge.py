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
fig = plt.figure(figsize=(4, 100), dpi=300)
ax1 = fig.add_subplot(511)
ax2 = fig.add_subplot(512)
ax3 = fig.add_subplot(513)
ax4 = fig.add_subplot(514)
ax5 = fig.add_subplot(515)

ax1.plot(data1x, data1y)
ax2.plot(data2x, data2y)
ax3.plot(data3x, data3y)
ax4.plot(data4x, data4y)
ax5.plot(data5x, data5y)

ax1.set_xlim([100, 1000])
ax2.set_xlim([100, 1000])
ax3.set_xlim([100, 1000])
ax4.set_xlim([100, 1000])
ax5.set_xlim([100, 1000])

plt.show()