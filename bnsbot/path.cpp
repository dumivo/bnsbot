#include "path.h"
#include "coord.h"
#include "bns.h"
#include "bot.h"
#include "action.h"

#include <Windows.h>
#include <vector>
#include <ctime>

using namespace bot;


bot::Path::Path() {
}

Path::Path(std::vector<coord::Coord> path) {
	path_ = path;
	dirty_ = 0;
}

bot::Path::Path(std::vector<coord::Coord> path, int dirty) {
	path_ = path;
	dirty_ = dirty;
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

	// Refresh keybd_device on every start of path. You might wanna remove this, future me.
	//bns_instance->RefreshKeybdDevice();

	size_t i = 0;
	int stuck_counter = 0;
	bool retry;
	while (i < path_.size()) {
		if (bot::GetState() == bot::Suspended) {
			continue;
		}
		if (bot::GetState() == bot::Off) {
			break;
		}
		// Update player address as we could be inside a loading screen.
		uintptr_t player = bns_instance->GetPlayerAddress();
		coord::Coord start_coords = bns_instance->GetPlayerCoord();

		// It's not safe doing this while in loading screen in general.
		// However you can use Move when the player structure updates.
		if (!player) {
			printf("[PATH] player is invalid. Retrying..\n");
			Sleep(2500);
			continue;
		}

		// Clear the last Move before going onto the next.. idk sometimes we get nasty crashes..
		/*while (bns_instance->PlayerIsBusy()) {
			printf("[PATH] Oh player is still busy moving.\n");
			Sleep(1000);
		}*/

		retry = false;
		auto &element = path_[i];
		

		bool good_move = bns_instance->SendMoveEasy(element);
		if (!good_move) {
			printf("[PATH] Move is invalid. Retrying..\n");
			Sleep(5000);
			continue;
		}

		// Block until player reached destination.
		// Retry if player is stuck.
		/*clock_t start_time = clock();
		double seconds_passed;
		while (bns_instance->PlayerIsBusy() && !retry) {
			Sleep(250);
			// Refreshing player address
			player = bns_instance->GetPlayerAddress();
			seconds_passed = (clock() - start_time) / CLOCKS_PER_SEC;
			if (seconds_passed >= 1 && player) {
				// Set retry to true if we haven't moved much after a second.
				float new_dist = coord::GetDistance(bns_instance->GetPlayerCoord(), start_coords);
				if (new_dist <= 50) {
					printf("Player is stuck? Distance=%f retrying..\n", new_dist);
					printf("This shouldn't happen and you're probably freezed.\n");
					printf("player=%p\n", (void *)player);
					bns_instance->RefreshKeybdDevice();
					bns_instance->RefreshPlayerAddress();
					retry = true;
				}
				start_time = clock();
				start_coords = bns_instance->GetPlayerCoord();
			}
		}

		if (i == path_.size() - 1 && dirty_ == 4) {
			printf("Sleeping\n");
			Sleep(50000);
			dirty_ = 0;
		}*/

		// Just sleep some time and check if we reached our destination...
		// Sleep the amounf of time you'd expect to reach the destination by doing some maths..
		const float velocity = 248;
		if (i == 0) {
			last_pos_ = bns_instance->GetPlayerCoord();
		}
		else {
			last_pos_ = path_[i - 1];
		}
		float distance = coord::GetDistance(last_pos_, element);
		float time_to_sleep = (distance / velocity);
		printf("Destination in %fs\n", time_to_sleep);
		Sleep((int) (time_to_sleep * 1000));

		// Check if we're actually at our destination
		// Loading screens somehow trigger the arrived dst flag..
		// We assume that you give us paths without obstacles so this here
		// below should only be for loading screens
		start_coords = bns_instance->GetPlayerCoord();
		distance = coord::GetDistance(start_coords, element);
		if (distance >= 200) {
			printf("[PATH] Player has reached destination, but not physically..\nDistance"
				"is %f\n", distance);
			retry = true;
			// Increment stack_counter every time the move failed.
			// If you got more than n stuck_counters, retry the previous move to unstuck
			if (stuck_counter >= 10) {
				stuck_counter = 0;
				printf("[PATH] Am stuck.. doing last move..\n");
				if (i >= 1) {
					i--;
				}
			}
			else {
				stuck_counter++;
			}
			
		}

		// TODO: Listen on external events like suspend or stop.
		if (!retry) {
			i++;
			stuck_counter = 0;
		}
		else {
			printf("Retrying move..\n");
			Sleep(2000);
			bns_instance->RefreshPlayerAddress();
		}

		Sleep(100);
	}
	return false;
}
