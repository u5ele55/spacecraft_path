
import matplotlib.pyplot as plt

file = open(r"out.txt", 'r')
data = file.read().split('\n')

xs,ys,zs = [], [], []
xd,yd,zd = [], [], []
xr,yr,zr = [], [], []
ax = plt.figure(1).add_subplot(projection='3d')
cnt = 0

N = int(data[0])

for i in range(1,N+1):
    x,y,z = [float(a) for a in data[i][1:-1].split()]
    xr.append(x)
    yr.append(y)
    zr.append(z)

enable_eci = True

for c in data[N+1:-1]:
    x,y,z = [float(a) for a in c.split()]
    if not enable_eci or cnt % 2:
        xd.append(x)
        yd.append(y)
        zd.append(z)
    else:
        xs.append(x)
        ys.append(y)
        zs.append(z)
    cnt += 1

#ax.plot(xs, ys, zs, label='SC trajectory eci')
ax.plot(xd, yd, zd, label='SC trajectory ecef', c='#FF0000')

import numpy as np

# earth
u, v = np.mgrid[0:2 * np.pi:30j, 0:np.pi:20j]
MAJOR_AXIS = 6378137; 
MINOR_AXIS = 6356752.3142; 
x_r = MAJOR_AXIS * np.cos(u) * np.sin(v)
y_r = MAJOR_AXIS * np.sin(u) * np.sin(v)
z_r = MINOR_AXIS * np.cos(v)

ax.plot_surface(x_r, y_r, z_r, cmap=plt.cm.YlGnBu_r, alpha=0.6)
# radiotelescopes
ax.scatter(xr, yr, zr, c='#FF0000', s=35, marker='o', alpha=1)
# trasse

tr = plt.figure(2)
img = plt.imread("/home/vshaganov/work/map.jpg")
plt.imshow(img, extent=[-np.pi/2, np.pi/2, -np.pi/2, np.pi/2])
xs = np.array(xs)
ys = np.array(ys)
zs = np.array(zs)

r = np.sqrt(xs**2 + ys**2 + zs**2)
r1 = np.sqrt(xs**2 + y**2)

alpha = 1/298.257
phi = np.arcsin(zs / np.sqrt((1-alpha)**2 * r1**2 + z**2))
lmbd = np.arctan(ys/xs)

# discontinuities
for i in range(len(lmbd)-1): 
    if lmbd[i] - lmbd[i+1] > 0:
        lmbd[i] = np.nan

plt.plot(lmbd, phi)


plt.show()