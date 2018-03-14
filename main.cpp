/**

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "omp.h"

#include "common.hpp"

void update_satellite(satellite_t *sat);

int main(int argc, char **argv) {
	int numthreads = omp_get_num_threads();

	printf("# Threads: %i\n", numthreads);

    #pragma omp parallel num_threads(5)
    {
        printf("Hello from %i of %i\n", omp_get_thread_num(), omp_get_num_threads());
    }

	int curItter = 0;
	int totalItter = 300 * 60; // total seconds to simulate TODO set this with cmd line arg as total minutes
	int numberSats = 2; // some way of loading our satilite orbit params from a config-like file (e.g. json, txt) will be needed

	satellite_t *satellites = (satellite_t*) malloc( numberSats * sizeof(satellite_t) );
	init_satellites(satellites, numberSats);

	for (int curItter = 0; curItter<totalItter; curItter++) {

		// printf("%f\n", satellites[0].trueAnomaly);
		// printf("%f\n", satellites[1].trueAnomaly);

		for (int i=0; i<numberSats; i++)
			update_satellite(&satellites[i]);
	}


}

void update_satellite(satellite_t *sat) {
	sat->trueAnomaly += 0.5; // TODO increment by relavent amount, based on orbit paramss
	sat->trueAnomaly = fmod(sat->trueAnomaly, 360.0);
}


/* TODO Append new info of satellites to output file format. */
void logStep(FILE *f, satellite_t *satellites) {

	// 'cur time' should be logged

	// Crital anamoly is the only thing that changes in relation to the spacecrafts orbit
	// (i.e. dont write all the other stuff unless it changes tick-to-tick)

}
