
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
