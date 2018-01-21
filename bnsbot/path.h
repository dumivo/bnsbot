#pragma once
#include <vector>

#include "command.h"
#include "coord.h"

namespace bot {
	class Path : public Command {
	public:
		Path();
		Path(std::vector<coord::Coord>);
		void SetPath(std::vector<coord::Coord> path);
		std::vector<coord::Coord> GetPath();
		bool Execute();
	protected:
		std::vector<coord::Coord> path_;
	};
}