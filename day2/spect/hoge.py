import matplotlib.pyplot as plt
# import pandas as pd 
import numpy as np 
file1 = '../a.txt'
file2 = '../i.txt'
file3 = '../u.txt'
file4 = '../e.txt'
file5 = '../o.txt'

data1x, data1y = np.loadtxt(file1, unpack=True)
data2x, data2y = np.loadtxt(file2, unpack=True)
data3x, data3y = np.loadtxt(file3, unpack=True)
data4x, data4y = np.loadtxt(file4, unpack=True)
data5x, data5y = np.loadtxt(file5, unpack=True)
# print(data1x)
fig = plt.figure(figsize=(4, 100), dpi=300)
ax1 = fig.add_subplot(111)
fig = plt.figure(figsize=(4, 100), dpi=300)
ax2 = fig.add_subplot(111)
fig = plt.figure(figsize=(4, 4), dpi=300)
ax3 = fig.add_subplot(111)
fig = plt.figure(figsize=(4, 4), dpi=300)
ax4 = fig.add_subplot(111)
fig = plt.figure(figsize=(4, 4), dpi=300)
ax5 = fig.add_subplot(111)
# ax2 = fig.add_subplot(111)
# ax3 = fig.add_subplot(111)
# ax4 = fig.add_subplot(111)
# ax5 = fig.add_subplot(111)

ax1.plot(data1x[:4000], data1y[:4000], linewidth=0.3)
ax2.plot(data2x[:4000], data2y[:4000], linewidth=0.3)
ax3.plot(data3x[:4000], data3y[:4000], linewidth=0.3)
ax4.plot(data4x[:4000], data4y[:4000], linewidth=0.3)
ax5.plot(data5x[:4000], data5y[:4000], linewidth=0.3)

ax1.set_xlim([100, 4000])
ax2.set_xlim([100, 4000])
ax3.set_xlim([100, 4000])
ax4.set_xlim([100, 4000])
ax5.set_xlim([100, 4000])
ax1.set_ylim([0, 1.5])
ax2.set_ylim([0, 1.5])
ax3.set_ylim([0, 1.5])
ax4.set_ylim([0, 1.5])
ax5.set_ylim([0, 1.5])
ax1.grid(which='both', linestyle='--', linewidth=0.2)
plt.minorticks_on()

ax2.grid(which='both', linestyle='--', linewidth=0.2)
plt.minorticks_on()

ax3.grid(which='both', linestyle='--', linewidth=0.2)
plt.minorticks_on()

ax4.grid(which='both', linestyle='--', linewidth=0.2)
plt.minorticks_on()

ax5.grid(which='both', linestyle='--', linewidth=0.2)
plt.minorticks_on()

plt.show()