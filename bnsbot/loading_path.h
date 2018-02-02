#pragma once
#include "path.h"
#include <vector>

namespace bot {
	// Extends Path for the sole purpose of handling loading screen
	// as the last destination tends to be moving into one.
	class LoadingPath : public Path {
		coord::Coord loading_move_;
		bool sleepy_ = false;
	public:
		LoadingPath(std::vector<coord::Coord>);
		LoadingPath(std::vector<coord::Coord>, bool sleepy);

		bool Execute();
	};
}
