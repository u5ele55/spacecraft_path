from trajectory import *
from geoid import GeoidDrawer
from radiotelescopes import TelescopesDrawer

MAJOR_AXIS = 6378137; 
MINOR_AXIS = 6356752.3142; 

if __name__ == '__main__':
    ax = plt.figure(1).add_subplot(projection='3d')
    trdr = TrajectoryDrawer(r"trajectory.txt")
    geoidDr = GeoidDrawer(MINOR_AXIS, MAJOR_AXIS)
    radioDr = TelescopesDrawer(r"telescopes.txt")

    geoidDr.draw(ax)
    radioDr.draw(ax)
    trdr.prepareTrasse()
    trdr.draw(ax)
    plt.show()