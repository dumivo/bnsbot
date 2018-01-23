#include "coord.h"
#include <stdio.h>
#include <math.h>

//#define COORD_SHOW_DEBUG_MESSAGES 0

float coord::GetDistance(Coord a, Coord b) {
#ifdef COORD_SHOW_DEBUG_MESSAGES
	printf("a = (%f, %f, %f)\nb = (%f, %f, %f)\n", a.x, a.y, a.z, b.x, b.y, b.z);
#endif
	return sqrtf(powf(a.x - b.x, 2) + 
		powf(a.y - b.y, 2) + 
		powf(a.z - b.z, 2));
}
