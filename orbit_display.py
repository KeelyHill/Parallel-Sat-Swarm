import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def WireframeSphere(centre=[0.,0.,0.], radius=1.,
                    n_meridians=20, n_circles_latitude=None):
    """https://stackoverflow.com/questions/40460960/how-to-plot-a-sphere-when-we-are-given-a-central-point-and-a-radius-size
    Create the arrays of values to plot the wireframe of a sphere.

    Examples
    --------
    >>> fig = plt.figure()
    >>> ax = fig.gca(projection='3d')
    >>> ax.set_aspect("equal")
    >>> sphere = ax.plot_wireframe(*WireframeSphere(), color="r", alpha=0.5)
    >>> fig.show()
    """
    if n_circles_latitude is None:
        n_circles_latitude = max(n_meridians/2, 4)
    u, v = np.mgrid[0:2*np.pi:n_meridians*1j, 0:np.pi:n_circles_latitude*1j]
    sphere_x = centre[0] + radius * np.cos(u) * np.sin(v)
    sphere_y = centre[1] + radius * np.sin(u) * np.sin(v)
    sphere_z = centre[2] + radius * np.cos(v)
    return sphere_x, sphere_y, sphere_z


mpl.rcParams['legend.fontsize'] = 10

df = pd.read_csv('test_orbit.csv')

# df = df[:20000]
# print(df.head())

fig = plt.figure(1)
fig.clf()
# ax = Axes3D(fig)
ax = fig.gca(projection='3d')

# draw sphere
sphere = ax.plot_wireframe(*WireframeSphere(radius=6371), color="blue", alpha=0.5)

for index, pos in df.iterrows():
    x,y,z = float(pos['x']), float(pos['y']), float(pos['z'])

    if index % 400 == 0:
        ax.scatter(x,y,z, c='r')


ax.set_aspect('equal')

ax.set_zlim3d([-20000, 20000])
ax.set_ylim3d([-20000, 20000])
ax.set_xlim3d([-20000, 20000])

plt.draw()
plt.show()
