import matplotlib.pyplot as plt
# import pandas as pd 
import numpy as np 
file1 = 'spectrum_a.txt'
file2 = 'spectrum_i.txt'
file3 = 'spectrum_u.txt'
file4 = 'spectrum_e.txt'
file5 = 'spectrum_o.txt'

file1c = '../a.txt'
file2c = '../i.txt'
file3c = '../u.txt'
file4c = '../e.txt'
file5c = '../o.txt'

data1x, data1y = np.loadtxt(file1, skiprows=1,unpack=True)
data2x, data2y = np.loadtxt(file2, skiprows=1,unpack=True)
data3x, data3y = np.loadtxt(file3, skiprows=1,unpack=True)
data4x, data4y = np.loadtxt(file4, skiprows=1,unpack=True)
data5x, data5y = np.loadtxt(file5, skiprows=1,unpack=True)

data1xc, data1yc = np.loadtxt(file1c, unpack=True)
data2xc, data2yc = np.loadtxt(file2c, unpack=True)
data3xc, data3yc = np.loadtxt(file3c, unpack=True)
data4xc, data4yc = np.loadtxt(file4c, unpack=True)
data5xc, data5yc = np.loadtxt(file5c, unpack=True)
# print(data1x)
fig = plt.figure(figsize=(6.4,6.4), dpi=300)
ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)

# ax1.plot(data1x, data1y, linewidth=0.5, label='a')
# ax1.plot(data2x, data2y, linewidth=0.5, label='i')
# ax1.plot(data3x, data3y, linewidth=0.5, label='u')
# ax1.plot(data4x, data4y, linewidth=0.5, label='e')
# ax1.plot(data5x, data5y, linewidth=0.5, label='o')

ax1.plot(data1xc[:200], data1yc[:200], linewidth=0.5, label='a')
ax1.plot(data2xc[:200], data2yc[:200], linewidth=0.5, label='i')
ax1.plot(data3xc[:200], data3yc[:200], linewidth=0.5, label='u')
ax1.plot(data4xc[:200], data4yc[:200], linewidth=0.5, label='e')
ax1.plot(data5xc[:200], data5yc[:200], linewidth=0.5, label='o')

ax2.plot(data1xc[201:402], data1yc[201:402], linewidth=0.5, label='a')
ax2.plot(data2xc[201:402], data2yc[201:402], linewidth=0.5, label='i')
ax2.plot(data3xc[201:402], data3yc[201:402], linewidth=0.5, label='u')
ax2.plot(data4xc[201:402], data4yc[201:402], linewidth=0.5, label='e')
ax2.plot(data5xc[201:402], data5yc[201:402], linewidth=0.5, label='o')

ax1.set_xlim([50, 1000])
ax1.set_ylim([0, 4500])

ax2.set_xlim([1000, 2000])
ax2.set_ylim([0, 500])

plt.legend()
plt.show()