import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from WireframeSphere import WireframeSphere


mpl.rcParams['legend.fontsize'] = 10

df = pd.read_csv('output.txt', names=['logItter', 'time', 'satid', 'trueA', 'x', 'y', 'z'])

# df = df[:20000]
# print(df.head())

fig = plt.figure(1)
fig.clf()
# ax = Axes3D(fig)
ax = fig.gca(projection='3d')

# draw sphere
sphere = ax.plot_wireframe(*WireframeSphere(radius=6371), color="blue", alpha=0.5)

for index, row in df.iterrows():

    if row['satid'] not in [0, 1] or row['logItter'] % 10 != 0:
        continue

    x,y,z = float(row['x']), float(row['y']), float(row['z'])

    color = 'rgb'[int(row['satid'])]
    ax.scatter(x,y,z, c=color)


# ax.scatter(10210, 964, 0.00, c='r')
# ax.scatter(-11, 16391, 0.00, c='b')
# ax.scatter(-20986, 9140, 0.00, c='g')

ax.set_aspect('equal')

ax.set_zlim3d([-20000, 20000])
ax.set_ylim3d([-20000, 20000])
ax.set_xlim3d([-20000, 20000])

plt.draw()
plt.show()
