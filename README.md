# Parallel Sat Swarm Orbit

![Testing 3D coords](https://github.com/KeelyHill/Parallel-Sat-Swarm/blob/master/img/test-3d-coords-orbit.png)
---------

## Running
Requires a C++ compiler that supports OpenMP.

Modify the Makefile or `CC` if needed.

On Mac: `$ make build`

Other: `$ make linux`

This creates an executable.  
`$ ./main.out` will run with default options.

## Command Line Options

- `time (-t)`: time to simulate (in minutes)

- `logfreq (-lf)`: simulation seconds between output logging (larger == faster)

- `in (-i)`: input file path

- `threads (-n)`: number of threads to run, default is optimal

- `help (-h)`: Usage display

**Example usage** (~1 week, logging positions every minute):  
`$ ./main.out -t 10000 -lf 60 -i inputs/polar1.txt`

## Input file
Each line is a satellite. Orbital elements are comma separated. Blank lines okay, create full line comments with `#`.

- semi-major axis
- eccentricity
- inclination
- right ascension (Ω)
- argument of periapsis (ω)
- true anomaly (f)

`a, e, i, rAscen(Ω), argPeri(ω), trueAnomaly(f)`

### Example:
```
# Two eccentric orbits
21000,0.4,0,6,0,0
15345,0.5,0,12,0,90
```

See `inputs/` directory for more, larger examples.


## Future Work
- Model ground stations _on_ the planet sphere and have them rotate with the planet.

- Atmosphere interaction (probably with micro maneuvers every tick). Requires a form and mass for each satellite. This project assumes station keeping, thus &Delta;V required do attitude control could also be modeled.

- Network simulations, not just generic communication line of sight. (This is a whole project in itself, but this can provide the backbone.) For example, could vary traffic based on population and time of day of location passing over. Can help understand what kind of communication hardware is needed for a swarm task.

- Use to calculate earth imagery percent-daily coverage given a certain focal length (thus field of view) of a lens.

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
