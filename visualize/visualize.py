from trajectory import *
from geoid import GeoidDrawer
from radiotelescopes import TelescopesDrawer
from designations import DesignationsDrawer

MAJOR_AXIS = 6378137; 
MINOR_AXIS = 6356752.3142; 

if __name__ == '__main__':
    ax = plt.figure(1).add_subplot(projection='3d')
    trdr = TrajectoryDrawer(r"trajectory.txt", MINOR_AXIS, MAJOR_AXIS)
    geoidDr = GeoidDrawer(MINOR_AXIS, MAJOR_AXIS)
    radioDr = TelescopesDrawer(r"telescopes.txt")
    desDr = DesignationsDrawer(r"designations.txt")

    geoidDr.draw(ax)
    radioDr.draw(ax)
    trdr.prepareTrasse()
    trdr.draw(ax)

    desDr.draw()

    plt.show()