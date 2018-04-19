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

int main(int argc, char **argv) {
	FILE *fOut;

    fOut = fopen ("output.txt", "w");
    if (fOut == NULL) {
      fOut = fopen("output.txt", "wb");
    }

	char buf[65536]; // 2^16, big buffer prevents constantly writing to file.
	setvbuf(fOut, buf, _IOFBF, sizeof(buf));

	char *inputFileName = (char *)"input.txt";

	// Defaults with no params
	int numThreads = 0; // 0 default is automatic optimal (OpenMP)
	int totalItter = (12 * 60 * 60) / DELTA_TIME; // total itterations to simulate
	int secondBetweenOutputLog = 60; // simulated seconds between file log (smaller->slower)


    for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-time") == 0 || strcmp(argv[i], "-t") == 0) {
			// Set time in minutes
			char* totalTimeStr = argv[i + 1];
			totalItter = (atoi(totalTimeStr) * 60) / DELTA_TIME;
			// printf("totalItter = %i\n", totalItter);
		}
		else if (strcmp(argv[i], "-logfreq") == 0 || strcmp(argv[i], "-lf") == 0) { // help string
			char* str = argv[i + 1];
			secondBetweenOutputLog = atoi(str);
		}
		else if (strcmp(argv[i], "-in") == 0 || strcmp(argv[i], "-i") == 0) { // help string
			inputFileName = argv[i + 1];
		}
		else if (strcmp(argv[i], "-threads") == 0 || strcmp(argv[i], "-n") == 0) {
			// Set number of threads
			char* numThreadsStr = argv[i + 1];
            numThreads = atoi(numThreadsStr);
			printf("numThreads = %d\n", numThreads);
		}
        else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) { // help string
		    printf("\nUsage: ./main [-time t (minutes)] [-logfreq lf (seconds)] [-threads n (default optimum)]\n\n");
            return 0;
		}
	}

	/* Initing */

	int numberSats = 0;

	// printf("Size of sat_t: %lu\n", sizeof(satellite_t));
	printf("\n** Initing Sats ** %f\n", read_timer());

	printf("Reading: %s\n", inputFileName);
	satellite_t *satellites = loadCSVConfig(inputFileName, &numberSats);
	if (satellites == NULL) {
		printf("File `%s` could not be read (probably does not exist).\n", inputFileName);
		return 0;
	}

	/// Statistics Variables:

	// counting average of unique lines of sight
	float total_lineOfSightSum = 0; // over all sim time
	int lineOfSightSum = 0; // per tick reduction

	int total_ClosePasses = 0; // over all sim time
	int closePassesSum = 0; // per tick reduction

    /* Simulating */

    printf("** Starting Simulation (of %.0f min) with %i satellites ** %f\n", (totalItter*DELTA_TIME)/60.0, numberSats, read_timer());

    const int freqPercentCount = (float)totalItter * FREQ_PERCENT_PRINT;
	const int freqOutputLogComparator = secondBetweenOutputLog / DELTA_TIME; // curItter % _ == 0, do a log

    int curItter = 0;
	int curSimTime = 0; // curItter * DELTA_TIME
	for (int curItter = 0; curItter<totalItter; curItter++) {
        curSimTime = curItter * DELTA_TIME;

		/* Update satellite orbital positions */

		lineOfSightSum = 0;
		closePassesSum = 0;

		// Location comparisons and statistics
		double x_eci, y_eci, z_eci;
        #pragma omp parallel for num_threads(numThreads) private (x_eci, y_eci, z_eci) reduction(+:lineOfSightSum) reduction(+:closePassesSum)
		for(int i=0; i<numberSats; i++) {

			double distance = -1;
			satellites[i].getECI_XYZ(x_eci, y_eci, z_eci);

			for(int j = i+1; j<numberSats; j++) { // note: j = i+1, O(NlogN) -- counting unique connections
				lineOfSightSum += satellitesHaveLineOfSight(x_eci, y_eci, z_eci, &satellites[j], distance);

				if (distance < 100 && distance >= 0) { // TODO make cutoff #define or a cmd line arg
					closePassesSum += 1;
				}
			}

        }

		// Update locations
		#pragma omp parallel for num_threads(numThreads)
		for(int i=0; i<numberSats; i++) {
		    update_satellite(&satellites[i], DELTA_TIME);
        }

		// Terminal percent progress indicator
        #pragma omp master
        if (curItter % freqPercentCount == 0) {
            printf("| %i%%\n", (int)(curItter/(float)totalItter * 100));
        }

		// Loger (with freqOutputLogComparator)
		#pragma omp master
		if (curItter % freqOutputLogComparator == 0) {
			for(int i=0; i<numberSats; i++) {
				satellites[i].getECI_XYZ(x_eci, y_eci, z_eci);
				logStep(fOut, curSimTime, &satellites[i], i, x_eci, y_eci, z_eci);
			}
			fflush(fOut);
		}

		#pragma omp master
		{
			total_ClosePasses += closePassesSum;
			closePassesSum = 0;
			total_lineOfSightSum += lineOfSightSum/(float)numberSats;
		}

	} // end main simulation itter for

	/* Report statistics */
	printf("** End Simulation ** took %f sec.\n", read_timer());

	printf("Avg. Number lines of sight (per sat). %f\n", total_lineOfSightSum/(double)totalItter);
	printf("Total Close Passes %i\n", total_ClosePasses);

	/* Clean up */

	free(satellites);

	// fprintf(fOut, "EOF");
	fflush(fOut);
    fclose(fOut);
}

void update_satellite(satellite_t *sat, double delta_time) {
    double mean_anoml = sat->true_to_mean_anoml(); // M_0

    // M_1 = M_0 + mean_motion + delta-t(s)
    mean_anoml += sat->calc_mean_motion() * delta_time;

    sat->trueAnomaly = sat->mean_to_true_anoml(mean_anoml); // new true anomaly
}

/* Append new info of a satellite to output file format. */
void logStep(FILE *f, int &simTime, satellite_t *sat, int &satItter, double &x_eci, double &y_eci, double &z_eci) {
	static int logItter = 0;
	static int lastSimTime;

	if (lastSimTime != simTime) {
		lastSimTime = simTime;
		logItter += 1; // increments by 1 for each set of points (for easier visualization/categorization)
	}

	// true anamoly is the only thing that changes in relation to the spacecrafts orbit
	// (i.e. dont write all the other stuff unless it changes tick-to-tick)

	/* For now: tick, sat_id, true anamoly, x, y, z */
    fprintf(f, "%d,%d,%d,%0.2f,%0.1f,%0.1f,%0.1f\n", logItter, simTime, satItter, radToDegPos(sat->trueAnomaly), x_eci, y_eci, z_eci);
}
