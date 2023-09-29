
import matplotlib.pyplot as plt
import numpy as np

class TrajectoryDrawer:
    def __init__(self, trajectoryFilename):
        self.fileTrajectory = open(trajectoryFilename, 'r')
        self.drawTrasse = False
        self.withECI = True

    def prepareTrasse(self):
        self.drawTrasse = True

    def draw(self, ax):
        data = self.fileTrajectory.read().split('\n')
        xs,ys,zs = [], [], []
        xd,yd,zd = [], [], []
        cnt = 0
        
        for c in data[:-1]:
            x,y,z = [float(a) for a in c.split()]
            if not self.withECI or cnt % 2:
                xd.append(x)
                yd.append(y)
                zd.append(z)
            else:
                xs.append(x)
                ys.append(y)
                zs.append(z)
            cnt += 1
        # Scaling
        to_all = lambda f, ar1, ar2, ar3: f(f(ar1), f(ar2), f(ar3))
        l = to_all(min, xd, yd, zd)
        u = to_all(max, xd, yd, zd)

        ax.set_xlim3d(l, u)
        ax.set_ylim3d(l, u)
        ax.set_zlim3d(l, u)

        ax.plot(xd, yd, zd, label='SC trajectory ecef', c='#FF0000')

        # trasse

        if not self.drawTrasse:
            return

        plt.figure()
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

#ax.plot(xs, ys, zs, label='SC trajectory eci')
