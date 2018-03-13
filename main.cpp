/**

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// #include "omp.h"

#include "common.hpp"

void update_satellite(satellite_t *sat);

int main(int argc, char **argv) {
	// int numthreads = omp_get_num_threads();

	// printf("Hello. #: %i", numthreads);

	// #pragma omp parallel {
  		printf("Hello world!\n");
	// }


	int totalItter = 50 * 60; // total seconds to simulate TODO set this with cmd line arg as total minutes
	int numberSats = 1;

	satellite_t *satellites = (satellite_t*) malloc( numberSats * sizeof(satellite_t) );
	init_satellites(satellites, numberSats);


	for (int i = 0; i<totalItter; i++) {

		printf("%f\n", satellites[0].trueAnomaly);

		update_satellite(&satellites[0]);
	}


}

void update_satellite(satellite_t *sat) {
	sat->trueAnomaly += 0.5; // TODO increment by relavent amount, based on orbit paramss
	sat->trueAnomaly = fmod(sat->trueAnomaly, 360.0);
}


/* TODO Append new info of satellites to output file format. */
void logStep(FILE *f, satellite_t *satellites) {

}
