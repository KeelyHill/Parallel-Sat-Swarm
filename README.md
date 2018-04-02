# Parallel Sat Swarm Orbit

![Testing 3D coords](https://github.com/KeelyHill/Parallel-Sat-Swarm/blob/master/img/test-3d-coords-orbit.png)
---------

## 2 April 2018 Programming Progress

Orbit simulation code done

Major functions done:
 - `double mean_to_eccentric_anomaly(double M, double tolerance=1e-14)`
 - `void getECI_XYZ(double &ret_x, double &ret_y, double &ret_z)`

Can now simulate N-sats (in parallel)
	They don't do anything other than orbit

Create input file loader for testing various configurations.

Need some kind of output

Now doing stretch things (whatever we can do by the end).
	- Model communications somehow
	- Maybe model data production (e.g. pictures)
	- Take statistics
	- Track close approaches
	- Ideas from class?
