#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string>

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


/** Given a file name, parse, malloc, and return a satellite_t * array.

Plain CSV Format, angle values in deg converted to rad (the satellite_t struct):
a,e,i,rAscen,argPeri,trueAnomaly

Ex: 25600, 0.6, 0,0,0, 130

Will Segmentation Fault if a line does not conform to the format.

Do not parallelize!
*/
satellite_t * loadCSVConfig(const char * fileName) {
    unsigned int number_of_lines = 0;
    FILE *infile = fopen(fileName, "r");

    int ch;
    // count number of line to init satillites array
    while (EOF != (ch=getc(infile)))
        if ('\n' == ch)
            ++number_of_lines;

    // init array
    satellite_t *satellites = (satellite_t*) malloc( number_of_lines * sizeof(satellite_t) );

    rewind(infile); // go to start of file (now that we've counted the lines)

    unsigned int line_num = 0;
    char line[1024];
	while (fgets(line, 1024, infile)) { // read line by line

        // grab & convert each comma separated value
        char *pt;

        pt = strtok (line, ",");
        satellites[line_num].a = atof(pt);
        pt = strtok (NULL, ",");
        satellites[line_num].e = atof(pt);
        pt = strtok (NULL, ",");
        satellites[line_num].i = degToRad(atof(pt));
        pt = strtok (NULL, ",");
        satellites[line_num].rAscen = degToRad(atof(pt));
        pt = strtok (NULL, ",");
        satellites[line_num].argPeri = degToRad(atof(pt));
        pt = strtok (NULL, ",");
        satellites[line_num].trueAnomaly = degToRad(atof(pt));

        line_num++; // next array index
	}

    return satellites;
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
