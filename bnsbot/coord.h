#pragma once

namespace coord {
	typedef struct coord_t {
		float x;
		float y;
		float z;
	} Coord;

	float GetDistance(const Coord &a, const Coord &b);
}