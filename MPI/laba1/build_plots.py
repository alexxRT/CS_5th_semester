import numpy as np
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# header syntax
GRID_SIZE  = 0
DX_VAL     = 1
DY_VAL     = 2
GRID_START = 3

fig = plt.figure()
ax  = plt.axes(projection="3d") 

def generate_grid(dx : float, dy : float, grid_size : int):
    x_size, y_size = (grid_size, grid_size)

    x_stop = dx*grid_size
    y_stop = dy*grid_size

    x = np.linspace(0, x_stop, grid_size)
    y = np.linspace(0, y_stop, grid_size)

    #coordinates grid
    X, Y = np.meshgrid(x, y)

    return X, Y

#using c calculate results and meshgrid
def read_grid(file_name : str):
    lines = []
    with open(file_name, "r") as file:
        lines = file.readlines()

    grid_id = lines[0].strip()[GRID_START]
    grid    = lines[grid_id:]

    for line in grid:
        line = line.strip()

    Z = np.array(grid, dtype=float)

    return Z

def animate_func(file_name : str, X, Y):
    Z = read_grid(file_name)

    global ax
    ax.clear()
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                cmap='viridis', edgecolor='none')
    
    ax.set_title('solution')


def run_build(file_name, X, Y):
    fig = plt.figure()
    ax  = plt.axes(projection="3d")
    ax.view_init(30, 45)

    ani = FuncAnimation(fig, animate_func(file_name, X, Y), interval = 1000)
    plt.show()


def init_build_plot(file_name : str):
    header = []
    inited = False

    #wait until backend is initilized
    while not inited:
        with open(file_name, "r") as file:
            header = file.readline().split()
            if (len(header)):
                inited = True

    grid_size = header[GRID_SIZE]
    dx = header[DX_VAL]
    dy = header[DY_VAL]

    X, Y = generate_grid(dx, dy, grid_size)

    with open(file_name, "w") as file:
        print("Build Plot Inited!")
        file.write("1")

    # start reading and building backend results
    run_build(file_name, X, Y)