
import matplotlib.pyplot as plt

file = open(r"out.txt", 'r')
coords = file.read().split('\n')

xs,ys,zs = [], [], []
ax = plt.figure().add_subplot(projection='3d')

for c in coords[:40000]:
    x,y,z = [float(a) for a in c.split()]
    
    xs.append(x)
    ys.append(y)
    zs.append(z)

ax.plot(xs, ys, zs, label='SC trajectory')

import numpy as np

u, v = np.mgrid[0:2 * np.pi:30j, 0:np.pi:20j]
R = 6.371e6
x = R * np.cos(u) * np.sin(v)
y = R * np.sin(u) * np.sin(v)
z = R * np.cos(v)
ax.plot_surface(x, y, z, cmap=plt.cm.YlGnBu_r)

plt.show()