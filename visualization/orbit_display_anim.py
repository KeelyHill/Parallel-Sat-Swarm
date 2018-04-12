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

def domeanim(i, scat):
    ax.view_init(45, (i/3)%360)

    frame = df.loc[df['logItter'] == i] # logItter increments each new set of points

    # clear and redraw static stuff
    ax.cla()
    ax.set_aspect('equal')

    ax.set_zlim3d([-20000, 20000])
    ax.set_ylim3d([-20000, 20000])
    ax.set_xlim3d([-20000, 20000])

    ax.plot_wireframe(*WireframeSphere(radius=6371), color="blue", alpha=0.5)

    for index, row in frame.iterrows():
        # print(row['x'])
        x,y,z = float(row['x']), float(row['y']), float(row['z'])

        color = ['b', 'g', 'r', 'c', 'm', 'y', 'k'][int(row['satid']) % 7]

        # ax.plot_wireframe(*WireframeSphere(radius=2000,center=[x,y,z], n_meridians=5), color=color, alpha=0.7)
        ax.scatter(x,y,z, c=color,marker="o")



scat = ax.scatter(0,0,0)
anim = animation.FuncAnimation(fig, domeanim, interval=10, frames=300, fargs=(scat,))

plt.tight_layout()

# print("Starting MP4 Render.")
# anim.save('animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
# print("MP4 Export Done!")

plt.show()
