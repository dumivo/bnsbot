#pragma once
#include <vector>

#include "command.h"
#include "coord.h"

namespace bot {
	class Path : public Command {
		std::vector<Coord> path_;

	public:
		Path(std::vector<Coord>);
		std::vector<Coord> GetPath();
		bool execute();
	};
}