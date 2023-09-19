
import matplotlib.pyplot as plt

file = open(r"out.txt", 'r')
coords = file.read().split('\n')

xs,ys,zs = [], [], []
xd,yd,zd = [], [], []
ax = plt.figure().add_subplot(projection='3d')
cnt = 0
for c in coords[:-1]:
    x,y,z = [float(a) for a in c.split()]
    if cnt % 2:
        xd.append(x)
        yd.append(y)
        zd.append(z)
    else:
        xs.append(x)
        ys.append(y)
        zs.append(z)
    cnt += 1

ax.plot(xs, ys, zs, label='SC trajectory eci')
ax.plot(xd, yd, zd, label='SC trajectory ecef', c='#FF0000')

import numpy as np

# sphere
u, v = np.mgrid[0:2 * np.pi:30j, 0:np.pi:20j]
R = 6.371e6
x_r = R * np.cos(u) * np.sin(v)
y_r = R * np.sin(u) * np.sin(v)
z_r = R * np.cos(v)

ax.plot_surface(x_r, y_r, z_r, cmap=plt.cm.YlGnBu_r)
# trasse
xs = np.array(xs)
ys = np.array(ys)
zs = np.array(zs)

r = np.sqrt(xs**2 + ys**2 + zs**2)
r1 = np.sqrt(xs**2 + y**2)

alpha = 1/298.257
phi = np.arcsin(zs / np.sqrt((1-alpha)**2 * r1**2 + z**2))
lmbd = np.arctan(ys/xs)

plt.plot(lmbd, phi)

plt.show()