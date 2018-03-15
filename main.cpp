/**

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "omp.h"

#include "common.hpp"

void update_satellite(satellite_t *sat);

int main(int argc, char **argv) {

	// Defaults with no params
    int numThreads = 2; // TODO set this with flag
	int totalItter = 100 * 60; // total seconds to simulate
	int numberSats = 2; // TODO some way of loading our satilite orbit params from a config-like file (e.g. json, txt) will be needed
    /*
    #pragma omp parallel num_threads(5)
    {
        printf("Hello from %i of %i\n", omp_get_thread_num(), omp_get_num_threads());
    }
	*/

    for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-threads") == 0 || strcmp(argv[i], "-T") == 0) {
			// Set number of threads
			char* numThreads = argv[i + 1];
			printf("numThreads = %s\n", numThreads);
		}
		else if (strcmp(argv[i], "-time") == 0 || strcmp(argv[i], "-t") == 0) {
			// Set time in minutes
			char* totalIterStr = argv[i + 1];
			totalItter = atoi(totalIterStr) * 60;
			printf("totalItter = %i\n ", totalItter);
		}
        else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-h") == 0) { // help string
		    printf("\nUsage: ./main [-time t] [-sats s]\n\n");
            return 0;
		}
	}

    /** Simulating */

	satellite_t *satellites = (satellite_t*) malloc( numberSats * sizeof(satellite_t) );
	init_satellites(satellites, numberSats);

    printf("Size of sat_t: %lu\n", sizeof(satellite_t));

    int curItter = 0;
	for (int curItter = 0; curItter<totalItter; curItter++) {

		// printf("%f\n", satellites[0].trueAnomaly);
		// printf("%f\n", satellites[1].trueAnomaly);

		for (int i=0; i<numberSats; i++)
			update_satellite(&satellites[i]);
	}

}

void update_satellite(satellite_t *sat) {

    int x = 3;
    double y = 3.123/14;
    double z = (x + y)/3.1415 * 42;

	sat->trueAnomaly += 0.5; // TODO increment by relavent amount (calculate degree change based on R current pos in orbit)
	sat->trueAnomaly = fmod(sat->trueAnomaly, 360.0);
}


/* TODO Append new info of satellites to output file format. */
void logStep(FILE *f, satellite_t *satellites) {

	// 'cur time' should be logged

	// Crital anamoly is the only thing that changes in relation to the spacecrafts orbit
	// (i.e. dont write all the other stuff unless it changes tick-to-tick)

}
