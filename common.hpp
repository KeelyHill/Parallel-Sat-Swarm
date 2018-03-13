
#ifndef COMMON_HPP
#define COMMON_HPP

typedef struct {
	double a;
	double e;
	double i;
	double rightAscension;
	double someOtherAngel;
	double trueAnomaly; // Angle from perigee to the spacecraft’s position
} satellite_t;

void init_satellites(satellite_t *sats, int n) {
	for (int i = 0; i<n; i++) {
		sats[i].trueAnomaly = 0;
	}
}

#endif
