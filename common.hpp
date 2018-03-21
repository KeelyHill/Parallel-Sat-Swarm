#include <math.h>
#include <stdio.h>

#ifndef COMMON_HPP
#define COMMON_HPP

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#define radToDegPos(angleInRadians) fmod((((angleInRadians) * 180.0 / M_PI) + 360), 360)

#define EARTH_G 398600.4418
#define MAX_ITERATIONS 100

/** */
typedef struct {
	double a; // semimajor axis
	double e; // eccentricity
	double i; // Inclination
	double rightAscension;
	double argOfPerigee; // Argument of perigee
	double trueAnomaly; // [+/- rad] : Angle from perigee to the spacecraft’s position

	// this does not change with time, based only on semi-major axis (a)
	double calc_mean_motion() {
	 	static double a_cubed = this->a * this->a * this->a; // rad/sec TODO this could probably be static
		return sqrt(EARTH_G/a_cubed);
	}

	/** */
	double true_to_mean_anoml() {
		double f = trueAnomaly;

		// true to Ecc
		double e_squared = e * e;
		double E = atan2( sqrt(1 - e_squared) * sin(f), e + cos(f) );
		E = fmod(E, 2 * M_PI);

		// Ecc to mean
    	return E - e * sin(E);
	}

	/** */
	double mean_to_true_anoml(double M) {

		// mean to Ecc
		double E = mean_to_eccentric_anomaly(M);

		// Ecc to true
		return 2 * atan2( sqrt(1 + e) * sin(E / 2), sqrt(1 - e) * cos(E / 2) );
	}

	/** */
	double mean_to_eccentric_anomaly(double M, double tolerance=1e-14) {

		double Mnorm, E, E0, dE, count;
		double t1, t2, t3, t4, t5, t6;
		double e_cubed = e * e * e;

		Mnorm = fmod(M, 2 * M_PI);
		E0 = M + (-1 / 2 * e_cubed + e + ( (e * e) + 3 / 2 * cos(M) * e_cubed) * cos(M)) * sin(M);
		dE = tolerance + 1;

		count = 0;
		while (dE > tolerance) {
		    t1 = cos(E0);
		    t2 = -1 + e * t1;
		    t3 = sin(E0);
		    t4 = e * t3;
		    t5 = -E0 + t4 + Mnorm;
		    t6 = t5 / (1 / 2 * t5 * t4 / t2 + t2);
		    E = E0 - t5 / ((1 / 2 * t3 - 1 / 6 * t1 * t6) * e * t6 + t2);
		    dE = abs(E - E0);
		    E0 = E;
		    count += 1;
		    if (count == MAX_ITERATIONS) {
				printf("Did not converge after %d iterations. (e=%f, M=%f)\n", MAX_ITERATIONS, e, M);
				return -1; // TODO make this better???
			}
		}

		return E;
	}

} satellite_t;


/** */
void init_satellites(satellite_t *sats, int n) {
	for (int i = 0; i<n; i++) {
		sats[i].trueAnomaly = degToRad(5); // aka: `f`

		sats[i].a = 25600; //Km
		sats[i].e = 0.6;
		sats[i].i = 0;
		sats[i].rightAscension = 0;
		sats[i].argOfPerigee = 0;

	}
}



#endif
