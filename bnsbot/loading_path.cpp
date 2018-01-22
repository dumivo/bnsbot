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

LoadingPath::~LoadingPath() {
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
			uintptr_t player = bns_instance->GetPlayer();
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

			bns_instance->Move(player, loading_move_.x, loading_move_.y, 
				loading_move_.z);

			Sleep(250);

			
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
			while (player && player == player_old) {
				// It's better to wait too long than too short.
				Sleep(2000);
				player = bns_instance->GetPlayer();
			}

#ifdef LOADING_PATH_DEBUG_MESSAGES
			printf("[LOADING_PATH] Address after loading screen: %p\n", player);
#endif // LOADING_PATH_DEBUG_MESSAGES

		} while (retry);
	} 
	Sleep(1000);
	return false;
}
