#include "path.h"
#include "coord.h"
#include "bns.h"
#include "bot.h"

#include <Windows.h>
#include <vector>
#include <ctime>

using namespace bot;

bot::Path::Path() {
}

Path::Path(std::vector<coord::Coord> path) {
	path_ = path;
}

void bot::Path::SetPath(std::vector<coord::Coord> path) {
	path_ = path;
}

std::vector<coord::Coord> Path::GetPath() {
	return path_;
}

bool Path::Execute() {
	if (path_.size() == 0) {
		return false;
	}
	bns::Bns *bns_instance = bns::Bns::getInstance();
	
	size_t i = 0;
	bool retry;
	while (i < path_.size()) {
		if (bot::GetState() == bot::Suspended) {
			continue;
		}
		// Update player address as we could be inside a loading screen.
		uintptr_t player = bns_instance->GetPlayer();
		coord::Coord start_coords = bns_instance->GetPlayerCoord();

		// It's not safe doing this while in loading screen in general.
		// However you can use Move when the player structure updates.
		if (!player) {
			printf("[PATH] player is invalid. Retrying..\n");
			Sleep(1000);
			continue;
		}
		retry = false;
		auto const &element = path_[i];
		printf("Moving to: (%f, %f, %f)\n", element.x, element.y, element.z);
		
		bns_instance->Move(player, element.x, element.y, element.z);

		if (!*(bool *)(player + 0x23C8)) {
			printf("Loading screen? Retrying..\n");
			Sleep(2000);
			continue;
		}

		// Block until player reached destination.
		// Retry if player is stuck.
		clock_t start_time = clock();
		double seconds_passed;
		while (bns_instance->PlayerIsBusy() && !retry) {
			Sleep(50);
			// Refreshing player address
			player = bns_instance->GetPlayer();
			seconds_passed = (clock() - start_time) / CLOCKS_PER_SEC;
			if (seconds_passed >= 1 && player) {
				// Set retry to true if we haven't moved much after a second.
				float new_dist = coord::GetDistance(bns_instance->GetPlayerCoord(), start_coords);
				if (new_dist <= 10) {
					retry = true;
				}
				start_time = clock();
				start_coords = bns_instance->GetPlayerCoord();
			}
		}

		// Check if we're actually at our destination
		// Loading screens somehow trigger the arrived dst flag..
		// We assume that you give us paths without obstacles so this here
		// below should only be for loading screens
		start_coords = bns_instance->GetPlayerCoord();
		float distance = coord::GetDistance(start_coords, element);
		if (distance >= 100) {
			// Assume that the last destination is through a portal and retry should be disabled.
			// We think that needs a more elegant solution right here..
			if (i < path_.size()) {
				printf("[PATH] Player has reached destination, but not physically..\n", distance);
				retry = true;
			}
		}

		// TODO: Listen on external events like suspend or stop.
		if (!retry) {
			i++;
		}
		else {
			printf("Retrying move..\n");
		}

		Sleep(1000);
	}
	return false;
}
