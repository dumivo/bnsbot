#pragma once
#include <vector>

#include "command.h"
#include "coord.h"

namespace bot {
	class Path : public Command {
		// 0 = nothing, 1 = skip robot cutscene at the end of path 2 = skip naksun
		int dirty_ = 0;
		coord::Coord last_pos_;
		void SkipCutscene(bool is_robot);
	public:
		Path();
		Path(std::vector<coord::Coord>);
		Path(std::vector<coord::Coord>, int dirty);
		void SetPath(std::vector<coord::Coord> path);
		std::vector<coord::Coord> GetPath();
		bool Execute();
	protected:
		std::vector<coord::Coord> path_;
	};
}