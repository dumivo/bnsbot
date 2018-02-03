#include "path.h"
#include "coord.h"
#include "bns.h"
#include "bot.h"
#include "action.h"

#include <Windows.h>
#include <vector>
#include <ctime>

using namespace bot;

void bot::Path::SkipCutscene(bool is_robot) {
	using namespace bns;
	Sleep(5000);
	char data[] = {
		0xB0, 0x98, 0x39, 0x41, 0x01, 0x00, 0x00, 0x00, 0x10, 0xE0, 0x5C, 0x0A, 0x01, 0x00, 0x00, 0x00,
		0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 83 robot
		0x49, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x57, 0x00, 0x30, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00 // 38 robot
	};
	Bns *bns = Bns::getInstance();
	printf("[EXIT_CUTSCENE] hm.. let's escape this bullshit\n");
	if (!is_robot) {
		data[0x18] = 0x84;
		data[0x2A] = 0x39;
	}
	bns->ExitLoadingScreen((uintptr_t)data);
	Sleep(1000);
}

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

		if (dirty_ == 3) {
			//Tab();
		}

		// Block until player reached destination.
		// Retry if player is stuck.
		clock_t start_time = clock();
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
		}

		// Check if we're actually at our destination
		// Loading screens somehow trigger the arrived dst flag..
		// We assume that you give us paths without obstacles so this here
		// below should only be for loading screens
		start_coords = bns_instance->GetPlayerCoord();
		float distance = coord::GetDistance(start_coords, element);
		if (distance >= 200) {
			printf("[PATH] Player has reached destination, but not physically..\nDistance"
				"is %f\n", distance);
			if (dirty_ != 0) {
				printf("[PATH] dirty is not zero.. so let's see if we should skip\n");
				printf("[PATH] Doing some dirty things now.\n");
				if (i == path_.size() - 1 || i == path_.size() - 2) {
					if (dirty_ == 1) {
						SkipCutscene(true);
					}
					else if (dirty_ == 2){
						SkipCutscene(false);
					}
				}
				else {
					printf("[PATH] No skip because i=%i and size-1=%i", i, path_.size() - 1);
				}
				
			}
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
			Sleep(3000);
		}

		Sleep(250);
	}
	return false;
}
