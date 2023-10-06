
import matplotlib.pyplot as plt
import numpy as np

class DesignationsDrawer:
    def __init__(self, designationsFilename):
        self.file = open(designationsFilename, 'r')
    
    def draw(self):
        filedata = self.file.read().split('\n')
        
        data = {}
        t_group = None

        def append(id, t,dist,azm,ang):
            data[id]['time'].append(t)
            data[id]['dist'].append(dist)
            data[id]['azimuth'].append(azm)
            data[id]['angle'].append(ang)

        for line in filedata[1:-1]:
            t,id,dist,azm,ang = [float(a) for a in line.split('\t')]
            
            id = int(id)
            if id not in data:
                data[id] = {'time': [], 'dist': [], 'azimuth': [], 
                            'angle': [], 'was': False}
            if not data[id]['was']:
                append(id, *[np.nan]*4)
            if len(data[id]['time']) > 2 and \
                abs(data[id]['azimuth'][-1] - data[id]['azimuth'][-2]) > 18 / np.pi:
                data[id]['azimuth'][-1] = np.nan
            if t != t_group:
                for _id in data:
                    if data[_id]['time'][-1] != t_group:
                        data[_id]['was'] = False
                t_group = t
            append(id, t,dist,azm,ang)
            data[id]['was'] = True
        
        for id in data:
            t = np.array(data[id]['time']) / 3600.0 
            fig, axs = plt.subplots(3, sharex=True)
            fig.suptitle(f'Station {id} designations')
            axs[0].plot(t, data[id]['dist'])
            axs[0].set_title("Distance")
            axs[1].plot(t, data[id]['azimuth'])
            axs[1].set_title("Azimuth")
            axs[2].plot(t, data[id]['angle'])
            axs[2].set_title("Angle")

            axs[0].set(ylabel='km')
            axs[1].set(ylabel='degrees')
            axs[2].set(ylabel='degrees')

            axs[2].set(xlabel='time, hours')
