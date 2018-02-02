#include "loading_path.h"
#include "bot.h"
#include <vector>

#define LOADING_PATH_DEBUG_MESSAGES 0

using namespace bot;

LoadingPath::LoadingPath(std::vector<coord::Coord> path) {
	path_ = path;
	loading_move_ = path_.back();
	path_.pop_back();
}

bot::LoadingPath::LoadingPath(std::vector<coord::Coord> path, bool sleepy) {
	path_ = path;
	loading_move_ = path_.back();
	path_.pop_back();
	sleepy_ = sleepy;
}

bool bot::LoadingPath::Execute() {
	Path::Execute();
#ifdef LOADING_PATH_DEBUG_MESSAGES
	printf("[LOADING_PATH] Base path finished. Doing last move..\n");
#endif // LOADING_PATH_DEBUG_MESSAGES
	while (bot::GetState() == bot::Suspended) {
		Sleep(500);
	}
	if (bot::GetState() == bot::Running) {
		bns::Bns *bns_instance = bns::Bns::getInstance();

		bool retry = false;
		do {
			retry = false;
			uintptr_t player = bns_instance->GetPlayerAddress();
			if (!player) {
				retry = true;
#ifdef LOADING_PATH_DEBUG_MESSAGES
				printf("[LOADING_PATH] invalid player.\n");
#endif // LOADING_PATH_DEBUG_MESSAGES
				Sleep(2000);
				continue;
			}

#ifdef LOADING_PATH_DEBUG_MESSAGES
			printf("[LOADING_PATH] Moving to (%f, %f, %f)\n", loading_move_.x, loading_move_.y,
				loading_move_.z);
#endif // LOADING_PATH_DEBUG_MESSAGES

			bns_instance->SendMoveEasy(loading_move_);

			Sleep(250);

			// To me from the future: yes I know this is cringy but.. you know I didn't have much time early on
			if (sleepy_) {
				Sleep(1000);
				return false;
			}
			
			// Wait for loading screen to pop up.
			while (bns_instance->PlayerIsBusy()) {
				Sleep(50);
			}


#ifdef LOADING_PATH_DEBUG_MESSAGES
			printf("[LOADING_PATH] Loading screen popped up!\n");
#endif // LOADING_PATH_DEBUG_MESSAGES

			// Wait until player address changes to a non-zero one.
			// That way we make sure that we don't access a bad address.
			uintptr_t player_old = player;
			bool was_null = false;
			clock_t start_time = clock();
			double seconds_passed;
			while (!player || player == player_old) {
				if (!player) {
					was_null = true;
#ifdef LOADING_PATH_DEBUG_MESSAGES
					printf("[LOADING_PATH] Player is null.\n");
#endif // LOADING_PATH_DEBUG_MESSAGES
				}
				else if (player == player_old && was_null == true) {
#ifdef LOADING_PATH_DEBUG_MESSAGES
					printf("[LOADING_PATH] Player address didn't change..\n");
#endif // LOADING_PATH_DEBUG_MESSAGES
					break;
				}

				// Just break after 60 seconds being idle..
				seconds_passed = (clock() - start_time) / CLOCKS_PER_SEC;
				if (seconds_passed >= 60) {
					start_time = clock();
					break;
				}

				// It's better to wait too long than too short.
				Sleep(2000);
				player = bns_instance->GetPlayerAddress();
			}

#ifdef LOADING_PATH_DEBUG_MESSAGES
			printf("[LOADING_PATH] Address after loading screen: %p\n", player);
#endif // LOADING_PATH_DEBUG_MESSAGES

		} while (retry);
	} 
	Sleep(5000);
	return false;
}
