#include <Windows.h>
#include <vector>

#include "bot.h"
#include "controller.h"
#include "sensors.h"
#include "coord.h"
#include "command.h"
#include "path.h"
#include "loading_path.h"

#define BOT_SHOW_DEBUG_MESSAGES 0

void bot::ChangeState(State state) {
	{
		std::lock_guard<std::mutex> lock(bot::mutex_bot_state);
		bot::bot_state = state;
	}
#if defined (BOT_SHOW_DEBUG_MESSAGES)
	printf("[BOT] Changed state to %i.\n", (int)state);
#endif
}

bot::State bot::GetState() {
	std::lock_guard<std::mutex> lock(bot::mutex_bot_state);
	return bot_state;
}

void bot::BotMain(LPVOID param) {
	// Setting things up before being ready.
	bns::Bns *bns_instance = bns::Bns::getInstance();
	hook::SetupHooks();
	// Using WinAPI thread because bns doesn't like C++11.:(
	// But somehow we can use std::lock_guard :thinking:.
	// Anyways, just don't use std::thread.
	ChangeState(bot::Ready);
	HANDLE controller_thread =
		CreateThread(0, 0, bot::ControllerThread, NULL, 0, 0);

	uintptr_t last_player = 0;
	uintptr_t current_player = 0;
	while (bot::GetState() != bot::Suspended) {
		while (bot::GetState() == bot::Running) {
			current_player = bns_instance->GetPlayer();
			if (current_player != last_player) {
				last_player = current_player;
				if (last_player != NULL) {
					float *coords = (float *)(current_player + 0x80);
					printf("player = %p (%f, %f, %f)\n", current_player, coords[0], coords[1], coords[2]);
				}
			}
			if (GetAsyncKeyState(VK_NUMPAD0)) {
				std::vector<coord::Coord> v = {
					{ -53739.0f, 6367.32f, -9611.62f },
					{ -53752.6f, 9084.30f, -9406.71f },
					{ -53260.7f, 9625.15f, -9112.16f }
				};
				// Coords for entering f13: -49460, 4811.15, -8524  

				//bns->Move(bns->GetPlayer(), -53460.5, 5289.35, 9982.39);
				Command *p = new LoadingPath(v);
				p->Execute();
				Sleep(500);
			}

			// Mushin second floor spawn location (-47519.125000, 9085.899414, -689.080811)

			// TODO: Implement functions doing these steps below.

			// Make "scripts" for specific bosses including moving,
			// killing, and looting said bosses.

			// Find a way (opcodes) to purchase orbs and spin the wheel.

			// Find a way to handle loading screens (either by using sensors)
			// or just waiting something like 30s each.

			// Enter Naksun
			// (loading screen)
			// use f13 ticket
			// enter f13
			// (loading screen)
			// Purchase orb
			// Spin wheel
			// move fujin
			// kill fujin
			// loot fujin
			// move robot
			// kill robot
			// loot robot
			// move naksun
			// kill naksun
			// loot naksun
			// exit naksun
			// (loading screen)
		}
		Sleep(500);
	}

	hook::RemoveHooks();
	ChangeState(Off);
	printf("[BOT] Waiting for Controller to quit.\n");
	WaitForSingleObject(controller_thread, INFINITE);
	CloseHandle(controller_thread);
	printf("[BOT] Quitting.\n");
}
