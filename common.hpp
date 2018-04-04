
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "satellite.hpp"

/** Returns the time (seconds) that have passed since first getting called. */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

/** TODO Write it and move to common.hpp
Given a file name, load a satellite_t * array.

Plain CSV Format, angle values in deg converted to rad (the satellite_t struct):
a,e,i,rightAscension,argOfPerigee,trueAnomaly

Ex: 25600, 0.6, 0,0,0, 130
*/
//satellite_t * loadCSVConfig(char * fileName) {
const char* loadCSVConfig(char* line, int num) {
	const char* tok;
	for (tok = strtok(line, ";");
		tok && *tok;
		tok = strtok(NULL, ";\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
	/*
	// satellite_t *satellites = (satellite_t*) malloc( numSats * sizeof(satellite_t) );
	int satArray[6];
	int deg = rad*180/math.pi;
	fPointer = fopen(fileName, "r");
	char singleLine[numSats];

	int i = 0;
	int numSatStart = 0;
	while(!feof(fPointer) && numSatStart < numSats){
	i++;
	if (i > 6){
	i = 0;
	}
	numSats++;
	satArray[i] = fgets(singleLine, numSats, fPointer);
	puts(singleLine);
	}

	fclose(fPointer);
	*/
}

void save( FILE *f, int num_sats, satellite_t *sats){
    static bool first = true; // only true when entering function for the first time
    if(first) {
        // fprintf(f, "%d %g\n", n, size );
        first = false;
    }
    for( int i = 0; i < num_sats; i++ )
        continue;
        // fprintf(f, "%g %g\n", p[i].x, p[i].y );
}
