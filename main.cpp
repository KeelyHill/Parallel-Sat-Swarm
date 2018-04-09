/**

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "omp.h"

#include "satellite.hpp"
#include "common.hpp"

#define FREQ_PERCENT_PRINT 0.1 // how often to print state of completion
#define DELTA_TIME 1  // seconds

void update_satellite(satellite_t *sat, double delta_time);
void logStep(FILE *f, int &simTime, satellite_t *sat, int &satItter, double &x_eci, double &y_eci, double &z_eci);
//satellite_t * loadCSVConfig(char * filename);

int main(int argc, char **argv) {
	FILE *fOut;

    fOut = fopen ("output.txt", "w");
    if (fOut == NULL) {
      fOut = fopen("output.txt", "wb");
    }

	char buf[65536]; // 2^16, big buffer prevents constantly writing to file.
	setvbuf(fOut, buf, _IOFBF, sizeof(buf));

	// Defaults with no params
	int numThreads = 0; // 0 default is automatic optimal (OpenMP)
	int totalItter = 12 * 60 * 60; // total seconds to simulate
	int numberSats = 300; // TODO some way of loading our satilite orbit params from a config-like file (e.g. json, txt) will be needed
	int secondBetweenOutputLog = 60; // seconds between log writes // TODO cmd line arg?

    for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-threads") == 0 || strcmp(argv[i], "-n") == 0) {
			// Set number of threads
			char* numThreadsStr = argv[i + 1];
            numThreads = atoi(numThreadsStr);
			printf("numThreads = %d\n", numThreads);
		}
		else if (strcmp(argv[i], "-time") == 0 || strcmp(argv[i], "-t") == 0) {
			// Set time in minutes
			char* totalIterStr = argv[i + 1];
			totalItter = atoi(totalIterStr) * 60;
			printf("totalItter = %i\n ", totalItter);
		}
        else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-h") == 0) { // help string
		    printf("\nUsage: ./main [-time t] [-threads n (default is optimum)]\n\n");
            return 0;
		}
	}


	/* Initing */

	// printf("Size of sat_t: %lu\n", sizeof(satellite_t));
	printf("** Initing Sats ** %f\n", read_timer());

//	const char * input_file_name = "input.txt";  // TODO input filename as cmd line argument
//	satellite_t *satellites = loadCSVConfig(input_file_name);

	satellite_t *satellites = (satellite_t*) malloc( numberSats * sizeof(satellite_t) );
	init_satellites(satellites, numberSats);

    /* Simulating */

    printf("** Starting Simulation (of %.0f min) ** %f\n", (totalItter*DELTA_TIME)/60.0, read_timer());

    int freqPercentCount = (float)totalItter * FREQ_PERCENT_PRINT;
	int freqOutputLogComparator = secondBetweenOutputLog / DELTA_TIME; // curItter % _ == 0, do a log

    int curItter = 0;
	int curSimTime = 0; // curItter * DELTA_TIME
	for (int curItter = 0; curItter<totalItter; curItter++) {
        curSimTime = curItter * DELTA_TIME;

		/* Update satellite orbital positions */

		double x_eci, y_eci, z_eci;
        #pragma omp parallel for num_threads(numThreads) private (x_eci, y_eci, z_eci)
		for(int i=0; i<numberSats; i++) {
		    update_satellite(&satellites[i], DELTA_TIME);

		    if(curItter % freqPercentCount == 0) {
				// satellites[i].getECI_XYZ(x_eci, y_eci, z_eci);
				// #pragma omp critical
				// logStep(fOut, &satellites[i], i, x_eci, y_eci, z_eci);
		    }
        }

        #pragma omp master
        if (curItter % freqPercentCount == 0) {
            printf("| %i%%\n", (int)(curItter/(float)totalItter * 100));
        }


		#pragma omp master
		if (curItter % freqOutputLogComparator == 0) {
			for(int i=0; i<numberSats; i++) {
				satellites[i].getECI_XYZ(x_eci, y_eci, z_eci);
				logStep(fOut, curSimTime, &satellites[i], i, x_eci, y_eci, z_eci);
			}
			fflush(fOut);
		}

    }


	/* Clean up */

	free(satellites);

    printf("** End Simulation ** took %f sec.\n", read_timer());

	// fprintf(fOut, "EOF");
	fflush(fOut);
    fclose(fOut);
}

void update_satellite(satellite_t *sat, double delta_time) {
    double mean_anoml = sat->true_to_mean_anoml(); // M_0

    // printf("true: %f == %f\n", sat->trueAnomaly, radToDegPos(sat->trueAnomaly));
    // printf("mean motion: %f mean anoml: %f\n", sat->calc_mean_motion(), mean_anoml);

    // M_1 = M_0 + mean_motion + delta-t(s)
    mean_anoml += sat->calc_mean_motion() * delta_time;

    sat->trueAnomaly = sat->mean_to_true_anoml(mean_anoml); // new true anomaly
}

/* TODO Append new info of satellites to output file format. */
void logStep(FILE *f, int &simTime, satellite_t *sat, int &satItter, double &x_eci, double &y_eci, double &z_eci) {
	// true anamoly is the only thing that changes in relation to the spacecrafts orbit
	// (i.e. dont write all the other stuff unless it changes tick-to-tick)

	/* For now i think:
		tick, sat_id, true anamoly, x, y, z
	*/

    fprintf(f, "time: %d  Sat %d: trueAnomoly %0.2f, x %0.2f, y %0.2f, z %0.2f\n", simTime, satItter, sat->trueAnomaly, x_eci, y_eci, z_eci);

}
