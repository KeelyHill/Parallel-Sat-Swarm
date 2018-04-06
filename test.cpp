/* For automated tests */


#include <stdio.h>

#include "satellite.hpp"

#define TEST_EARTH_RADIUS_SQUARED 40589641  // Km

bool test_lineIntersectsSphere() {

	double x1 = 10210;
	double y1 = 964;
	double z1 = 0;
	double x2,y2,z2;
	z2 = 0;

	// should NOT intersect
	x2 = -10;
	y2 = 16390;

	bool no = lineIntersectsSphere(x1,y1,z1,x2,y2,z2,TEST_EARTH_RADIUS_SQUARED);

	// YES interesect
	x2 = -20986;
	y2 = 9140;

	bool yes = lineIntersectsSphere(x1,y1,z1,x2,y2,z2,TEST_EARTH_RADIUS_SQUARED);

	return (no==false) && (yes==true);
}

int main() {
	printf("Testing...\n");

	bool test1 = test_lineIntersectsSphere();

	if (!test1) {
		printf("FAIL test_lineIntersectsSphere\n");
	}

	if (test1) {
		printf("PASS\n");
	}

}
