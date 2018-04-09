/** common.hpp

satellite_t struct and orbital mechanics math

Orbital algorithms adapted from:
https://github.com/RazerM/orbital/blob/master/orbital/utilities.py

*/

#include <math.h>
#include <stdio.h>

#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#define radToDegPos(angleInRadians) fmod((((angleInRadians) * 180.0 / M_PI) + 360), 360)

#define EARTH_G 398600.4418
#define EARTH_RADIUS 6371  // Km
#define EARTH_RADIUS_SQUARED 40589641  // Km
#define MAX_ITERATIONS 100

/** */
typedef struct {
	double a; // semimajor axis
	double e; // eccentricity
	double i; // Inclination
	double rAscen; // Ω
	double argPeri; // Argument of perigee ω
	double trueAnomaly; // [+/- rad] : Angle from perigee to the spacecraft’s position

	// this does not change with time, based only on semi-major axis (a)
	double calc_mean_motion() {
	 	double a_cubed = this->a * this->a * this->a; // rad/sec
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

	/** */
	void getECI_XYZ(double &ret_x, double &ret_y, double &ret_z) {
		double * f = &this->trueAnomaly;
		double radius_at_f = (this->a * (1 - this->e * this->e)) / (1 + this->e * cos(*f));
		// double vel = sqrt(EARTH_G * (2/radius_at_f - 1/this->a));

		// http://control.asu.edu/Classes/MAE462/462Lecture07.pdf
		double r_x_peri = radius_at_f * cos(*f);
		double r_y_peri = radius_at_f * sin(*f);
		double r_z_peri = 0;

		double R[3][3]; // TODO, maybe ballance this with memory and memory access time

		double sinsinomeg = sin(this->rAscen) * sin(this->argPeri);  // sin(Ω) * sin(ω)
		double coscosomeg = cos(this->rAscen) * cos(this->argPeri);  // cos(Ω) * cos(ω)
		double sincosomeg = sin(this->rAscen) * cos(this->argPeri);  // sin(Ω) * cos(ω)
		double cossinomeg = cos(this->rAscen) * sin(this->argPeri);  // cos(Ω) * sin(ω)
		double sin_i = sin(this->i);
		double cos_i = cos(this->i);

		R[0][0] = coscosomeg - sinsinomeg * cos_i;
		R[0][1] = -cossinomeg - sincosomeg * cos_i;
		R[0][2] = sin(this->rAscen) * sin_i;

		R[1][0] = sincosomeg + cossinomeg * cos_i;
		R[1][1] = -sinsinomeg + coscosomeg * cos_i;
		R[1][2] = -cos(this->rAscen) * sin_i;

		R[2][0] = sin(this->argPeri) * sin_i;
		R[2][1] = cos(this->argPeri) * sin_i;
		R[2][2] = cos_i;

		ret_x = r_x_peri * R[0][0] + r_y_peri * R[0][1] + r_z_peri * R[0][2];
		ret_y = r_x_peri * R[1][0] + r_y_peri * R[1][1] + r_z_peri * R[1][2];
		ret_z = r_x_peri * R[2][0] + r_y_peri * R[2][1] + r_z_peri * R[2][2];
	}

} satellite_t;


/** */
void init_satellites(satellite_t *sats, int n) {
	for (int i = 0; i<n; i++) {
		sats[i].trueAnomaly = degToRad(5); // aka: `f`

		sats[i].a = 25600; //Km
		sats[i].e = 0.6;
		sats[i].i = 0;
		sats[i].rAscen = 0;
		sats[i].argPeri = 0;

	}
}

/** May want this, leaving as blank for now. */
void random_init_satellites(satellite_t *sats, int n) {
	for (int i = 0; i<n; i++) {
		// sats[i].trueAnomaly = 0; // aka: `f`
		//
		// sats[i].a = 25600; //Km
		// sats[i].e = 0.6;
		// sats[i].i = 0;	// < 2pi rad
		// sats[i].rAscen = 0; // < 2pi rad
		// sats[i].argOfPerigee = 0; // < 2pi rad
	}
}

/** Returns true if the line (given by 2, 3D points) intersects a sphere with radius.
False if line never intersects or touches.

Must square the radius before calling (for better usage of immediates and macros).
*/
bool lineIntersectsSphere(double &x1, double &y1, double &z1, double &x2, double &y2, double &z2, double radius_squared) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;

	// quadratic components
	double a = dx*dx + dy*dy + dz*dz;
	double b = 2 * (dx*x1 + dy*y1 + dz*z1);
	double c = (x1*x1 + y1*y1 + z1*z1) - radius_squared;

	double determinant = (b*b) - 4 * a * c;
	return determinant >= 0; // intersects if determinant >= 0
}


/**

TODO shrink sphere to model atmosphere bouncing
TODO docstring

*/
bool satellitesHaveLineOfSight(satellite_t *one, satellite_t *two) {
	double x1_eci, y1_eci, z1_eci;
	double x2_eci, y2_eci, z2_eci;
	one->getECI_XYZ(x1_eci, y1_eci, z1_eci);
	two->getECI_XYZ(x2_eci, y2_eci, z2_eci);

	return !lineIntersectsSphere(x1_eci, y1_eci, z1_eci, x2_eci, y2_eci, z2_eci, EARTH_RADIUS_SQUARED);
}


#endif
