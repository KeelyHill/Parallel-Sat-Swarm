#include <math.h>

#ifndef COMMON_HPP
#define COMMON_HPP

#define EARTH_G 398600.4418

/** */
typedef struct {
	double a;
	double e;
	double i;
	double rightAscension;
	double someOtherAngel;
	double trueAnomaly; // Angle from perigee to the spacecraft’s position

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
		double E = mean_to_eccentric_anomaly(e, M);

		// Ecc to true
		return 2 * atan2( sqrt(1 + e) * sin(E / 2), sqrt(1 - e) * cos(E / 2) );
	}

	double mean_to_eccentric_anomaly(double M, double tolerance=1e-14) {

		return -1;
	}

} satellite_t;


/** */
void init_satellites(satellite_t *sats, int n) {
	for (int i = 0; i<n; i++) {
		sats[i].trueAnomaly = 0;
	}
}



#endif
