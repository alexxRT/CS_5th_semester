import numpy as np
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import sys
import matplotlib.animation as an

# header syntax
GRID_SIZE   = 0
STEP_VAL    = 1
GRID_START  = 2

read_step = 0

def generate_grid(grid_step: float, grid_size : int):
    x_stop = grid_step * grid_size
    y_stop = grid_step * grid_size

    x = np.linspace(0, x_stop, grid_size)
    y = np.linspace(0, y_stop, grid_size)

    #coordinates grid
    X, Y = np.meshgrid(x, y)

    return X, Y

#using c calculate results and meshgrid
def read_grid(file_name : str, position : int):
    lines = []
    with open(file_name, "r") as file:
        lines = file.readlines()

    grid_id   = int((lines[position].split())[GRID_START])
    grid_size = int((lines[position].split()[GRID_SIZE]))

    grid = [line.replace("\n", "").split() for line in lines[position + grid_id:position + grid_size + 1]]

    Z = np.array(grid, dtype=float)

    return Z

def animate_func(position, file_name : str, X, Y):
    Z = read_grid(file_name, position)

    ax = fig.axes

    ax[0].clear()
    ax[0].set_zlim(-1, 1)
    ax[0].plot_surface(X, Y, Z, rstride=1, cstride=1,
                cmap='viridis', edgecolor='none')
    
    ax[0].set_title('solution')


def init_field(file_name : str):
    header = str()

    with open("dump.txt", "r") as file:
        header = file.readline().split()

    grid_size = int(header[GRID_SIZE])
    grid_id   = int(header[GRID_START])
    global read_step
    read_step = grid_id + grid_size

    grid_step = float(header[STEP_VAL])

    X, Y = generate_grid(grid_step, grid_size)

    return X, Y


if __name__ == "__main__":
    file_name = "dump.txt"
    X, Y = init_field(file_name)
    frames = np.arange(0, 10000, read_step)

    fig = plt.figure()
    fig.set_label("Animation example")
    ax  = fig.add_subplot(projection="3d")

    anim = FuncAnimation(fig, animate_func, frames, fargs=(file_name, X, Y), interval=8)

    # save if only save flag is up
    if (len(sys.argv) > 1 and sys.argv[1] == "true"):
        wr_video = an.FFMpegWriter(60)
        anim.save("conseq.mp4", writer=wr_video)

    plt.show()