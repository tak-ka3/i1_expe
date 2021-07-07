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
fig1 = plt.figure(figsize=(4, 4), dpi=300)
ax1 = fig1.add_subplot(111)
fig2 = plt.figure(figsize=(4, 4), dpi=300)
ax2 = fig2.add_subplot(111)
fig3 = plt.figure(figsize=(4, 4), dpi=300)
ax3 = fig3.add_subplot(111)
fig4 = plt.figure(figsize=(4, 4), dpi=300)
ax4 = fig4.add_subplot(111)
fig5 = plt.figure(figsize=(4, 4), dpi=300)
ax5 = fig5.add_subplot(111)
# ax2 = fig.add_subplot(111)
# ax3 = fig.add_subplot(111)
# ax4 = fig.add_subplot(111)
# ax5 = fig.add_subplot(111)

ax1.plot(data1x[:4000], data1y[:4000], linewidth=0.3)
ax2.plot(data2x[:4000], data2y[:4000], linewidth=0.3)
ax3.plot(data3x[:4000], data3y[:4000], linewidth=0.3)
ax4.plot(data4x[:4000], data4y[:4000], linewidth=0.3)
ax5.plot(data5x[:4000], data5y[:4000], linewidth=0.3)

ax1.set_xlim([100, 2000])
ax2.set_xlim([100, 2000])
ax3.set_xlim([100, 2000])
ax4.set_xlim([100, 2000])
ax5.set_xlim([100, 2000])
ax1.set_ylim([0, 2.5])
ax2.set_ylim([0, 2.5])
ax3.set_ylim([0, 2.5])
ax4.set_ylim([0, 2.5])
ax5.set_ylim([0, 2.5])
ax1.grid(which='both', linestyle='--', linewidth=0.2)
ax1.set_title('a', loc='center', y = -0.38)
ax1.set_xlabel('Power spectrum')
ax1.set_ylabel('Frequency [Hz]')
plt.savefig('a.png')


ax2.grid(which='both', linestyle='--', linewidth=0.2)
ax2.set_title('i', loc='center', y = -0.38)
ax2.set_xlabel('Power spectrum')
ax2.set_ylabel('Frequency [Hz]')
plt.savefig('i.png')


ax3.grid(which='both', linestyle='--', linewidth=0.2)
ax3.set_title('u', loc='center', y = -0.38)
ax3.set_xlabel('Power spectrum')
ax3.set_ylabel('Frequency [Hz]')
plt.savefig('u.png')


ax4.grid(which='both', linestyle='--', linewidth=0.2)
ax4.set_title('e', loc='center', y = -0.38)
ax4.set_xlabel('Power spectrum')
ax4.set_ylabel('Frequency [Hz]')
plt.savefig('e.png')


ax5.grid(which='both', linestyle='--', linewidth=0.2)
ax5.set_title('o', loc='center', y = -0.38)
ax5.set_xlabel('Power spectrum')
ax5.set_ylabel('Frequency [Hz]')
plt.savefig('o.png')

plt.show()
