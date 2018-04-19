import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import numpy as np
import pandas as pd

from WireframeSphere import WireframeSphere

mpl.rcParams['legend.fontsize'] = 5

df = pd.read_csv('output.txt', names=['logItter', 'time', 'satid', 'trueA', 'x', 'y', 'z'])

fig = plt.figure(1, figsize=(11, 6), dpi=120)
fig.clf()
# ax = Axes3D(fig)
ax = fig.gca(projection='3d')

AXIS_BOUND = 13000  # default:20000

font = {'fontname':'Courier'}

def domeanim(i, scat):
    # ax.view_init(65, (i/6)%360) # orbit Z axis
    # ax.view_init(70-(i/10)%360, 45) # pivot XY plane
    ax.view_init(45-(i/12)%360, 90+(i/6)%360)

    frame = df.loc[df['logItter'] == i] # logItter increments each new set of points

    # clear and redraw static stuff
    ax.cla()
    ax.set_aspect('equal')

    ax.text2D(0.90, 0.95, "Time (min): %i" % (int(frame.iloc[0]['time'])/60), transform=ax.transAxes)

    ax.set_zlim3d([-AXIS_BOUND, AXIS_BOUND])
    ax.set_ylim3d([-AXIS_BOUND, AXIS_BOUND])
    ax.set_xlim3d([-AXIS_BOUND, AXIS_BOUND])

    ax.plot_wireframe(*WireframeSphere(radius=6371), color="blue", alpha=0.5)

    for index, row in frame.iterrows():
        # print(row['x'])
        x,y,z = float(row['x']), float(row['y']), float(row['z'])

        # color = ['b', 'g', 'r', 'c', 'm', 'y', 'k'][int(row['satid']) % 7]

        color = 'blue'
        if (row['satid']%6) == 2 or (row['satid']%6) == 3:
            color = 'm'
        if (row['satid']%6) == 4 or (row['satid']%6) == 5:
            color = 'red'

        ax.plot_wireframe(*WireframeSphere(radius=1200,center=[x,y,z], n_meridians=6), color=color, alpha=0.5)
        ax.scatter(x,y,z, c=color,marker="o")


scat = ax.scatter(0,0,0)
LENTH_SEC = 40
anim = animation.FuncAnimation(fig, domeanim, interval=10, frames=LENTH_SEC*30, fargs=(scat,))

plt.tight_layout()

print("Starting MP4 Render.")
anim.save('animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
print("MP4 Export Done!")

print("Showing live.")
plt.show()
