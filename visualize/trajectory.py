
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
        xs,ys,zs = [], [], [] # ECI
        xd,yd,zd = [], [], [] # ECEF
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

        # ECI trajectory
        ax.plot(xs, ys, zs, label='SC trajectory eci', c='#FF0000')
        ax.scatter(xs[0], ys[0], zs[0], label='Start', c='#FFFF00')

        # trasse
        if not self.drawTrasse:
            return

        fig = plt.figure()
        ax = fig.add_subplot()
        img = plt.imread("/home/vshaganov/work/map.jpg")
        plt.imshow(img, extent=[-180, 180, -180, 180])
        ax.set_aspect("auto")

        xd = np.array(xd)
        yd = np.array(yd)
        zd = np.array(zd)

        r1 = np.sqrt(xd**2 + yd**2)
        alpha = 1/298.257
        phi = np.arcsin(zd / np.sqrt((1-alpha)**2 * r1**2 + zd**2)) * 180 / np.pi
        lmbd = np.arctan(yd/xd) * 180 / np.pi

        # discontinuities
        for i in range(len(lmbd)-1): 
            if lmbd[i] - lmbd[i+1] > 0:
                lmbd[i] = np.nan

        ax.plot(lmbd, phi)
        ax.scatter(lmbd[0], phi[0], label='start', c="#00FF00")
        ax.scatter(lmbd[-1], phi[-1], label='end', c='#FF0000')
        ax.legend()
        

#ax.plot(xs, ys, zs, label='SC trajectory eci')
