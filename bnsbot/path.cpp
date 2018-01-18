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

bool Path::Execute() {
	bns::Bns *bns_instance = bns::Bns::getInstance();

	// player address shouldn't change within a path-command.
	uintptr_t player = bns_instance->GetPlayer();

	size_t i = 0;
	while (i < path_.size()) {
		auto const &element = path_[i];
		printf("Moving to: (%f, %f, %f)\n", element.x, element.y, element.z);
		bns_instance->Move(player, element.x, element.y, element.z);
		
		// Block until player reached destination.
		while (*(bool *)(player + 0x23C8)) {
			Sleep(50);
		}

		// TODO: Listen on external events like suspend or stop.
		i++;
		Sleep(1000);
	}

	/*for (auto const &element : path_) {
		bns_instance->Move(bns_instance->GetPlayer(),
			element.x,
			element.y,
			element.z);

		Sleep(1000);
	}*/
	return false;
}
