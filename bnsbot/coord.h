#pragma once

namespace coord {
	typedef struct coord_t {
		float x;
		float y;
		float z;
	} Coord;

	float GetDistance(Coord a, Coord b);
}