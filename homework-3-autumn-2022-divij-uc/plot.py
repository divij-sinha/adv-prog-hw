
import numpy as np
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
from celluloid import Camera
from matplotlib import collections



def save_anim(datfile, savefile):
    with open(datfile) as f:
        row = f.readline()
        n, iters, _ = [int(__) for __ in row.split()]
        data = f.readlines()
        arr = np.ndarray((iters,n,2))
        for iter in range(iters):
            iter_data = data[iter*n:(iter+1)*n]
            iter_data = [[float(__) for __ in _.split()[:(len(_)-1)]] for _ in iter_data]
            arr[iter] = iter_data
            
        max_num = np.max(np.abs(arr))    
        print("MAXNUM: ", max_num)
        max_num = 2

    fig = plt.figure() 
    ax = plt.axes(xlim =(-max_num,max_num),ylim =(-max_num, max_num))
    camera = Camera(fig)
    # hiding the axis details 
    plt.axis('off') 

    for i in range(iters):
        plt.scatter(arr[i,:,0], arr[i,:,1], color = "blue")
        camera.snap()
        
    animation = camera.animate()
    animation.save(savefile)# save the animation as mp4 video file 


def grid(datafile, savefile):
    with open(datafile) as f:
        rows = f.readlines()
    arr = np.ndarray((len(rows)-1,10))
    for i, row in enumerate(rows[1:-1]):
        row = [float(_) for _ in row.replace("\n","").split()]
        if len(row) == 8:
            row+= [np.nan, np.nan]
        arr[i] = row

    fig = plt.figure(figsize = (20,20), dpi = 600) 
    max_num = np.nanmax(np.abs(arr[:,1]))
    ax = plt.axes(xlim =(-max_num,max_num),ylim =(-max_num, max_num))


    a = collections.LineCollection(np.array([(arr[:,3],arr[:,4]),(arr[:,1],arr[:,1])]).transpose(), colors = matplotlib.cm.cool(arr[:,0]/np.max(arr[:,0])))
    b = collections.LineCollection(np.array([(arr[:,3],arr[:,4]),(arr[:,2],arr[:,2])]).transpose(), colors = matplotlib.cm.cool(arr[:,0]/np.max(arr[:,0])))
    c = collections.LineCollection(np.array([(arr[:,3],arr[:,3]),(arr[:,1],arr[:,2])]).transpose(), colors = matplotlib.cm.cool(arr[:,0]/np.max(arr[:,0])))
    d = collections.LineCollection(np.array([(arr[:,4],arr[:,4]),(arr[:,1],arr[:,2])]).transpose(), colors = matplotlib.cm.cool(arr[:,0]/np.max(arr[:,0])))

    plt.scatter(arr[:,8],arr[:,9], s=1)
    ax.add_collection(a)
    ax.add_collection(b)
    ax.add_collection(c)
    ax.add_collection(d)
    plt.savefig(savefile)

        



# save_anim("particles_serial.dat", "anim_serial_2.mp4")
save_anim("particles_bh.dat", "anim_bh.mp4")

grid("init_grid.dat","init_grid.png")
grid("final_grid.dat","final_grid.png")