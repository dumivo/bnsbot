#include "path.h"
#include "coord.h"
#include "bns.h"

#include <Windows.h>
#include <vector>

using namespace bot;

Path::Path(std::vector<Coord> path) {
	path_ = path;
}

std::vector<Coord> Path::GetPath() {
	return path_;
}

bool Path::execute() {
	bns::Bns *bns_instance = bns::Bns::getInstance();
	for (auto const &element : path_) {
		bns_instance->Move(bns_instance->GetPlayer(),
			element.x,
			element.y,
			element.z);

		Sleep(1000);
	}
	return false;
}
