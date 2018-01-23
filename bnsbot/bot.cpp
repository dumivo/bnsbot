#include <Windows.h>
#include <vector>

#include "bot.h"
#include "controller.h"
#include "sensors.h"
#include "coord.h"
#include "command.h"
#include "path.h"
#include "loading_path.h"
#include "action.h"
#include "combat.h"
#include "packets.h"
#include "buy_ticket.h"
#include "buy_entrance_ticket.h"
#include "loot.h"

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

		// Set up script
		Command *mushin_to_room = new LoadingPath(std::vector<coord::Coord> {
			{ -53739.0f, 6367.32f, -9611.62f },
			{ -53752.6f, 9084.30f, -9406.71f },
			{ -53260.7f, 9625.15f, -9112.16f }
		});
		Command *room_to_f13_1 = new Path(std::vector<coord::Coord> {
			{ -49689.570313f, 4815.287109f, -8794.195313f },
		});
		Command *buy_entrance_ticket = new BuyEntranceTicket();
		Command *room_to_f13_2 = new LoadingPath(std::vector<coord::Coord> {
			{ -49446.367188f, 4815.529297f, -8795.016602f },
		});
		Command *f13_to_fujin = new Path(std::vector<coord::Coord> {
			{-47519.023438f, 9901.060547f, 20080.927734f},
			{ -47511.371094f, 9648.577148f, 20081.759766f },
			{ -47519.023438f, 9901.060547f, 20080.927734f }
		});
		Command *kill = new CombatSpin();
		Command *loot = new Loot();
		Command *fujin_to_f14 = new Path(std::vector<coord::Coord> {
			{ -47506.312500f, 11562.365234f, 20261.689453f },
			{ -47518.339844f, 11742.772461f, 20271.408203f },
			{ -46772.253906f, 11723.500000f, 20268.568359f },
			{ -45880.183594f, 10840.580078f, 20854.720703f },
			{ -45864.867188f, 9225.890625f, 20877.144531f },
			{ -46674.367188f, 8474.617188f, 21411.847656f },
			{ -47554.085938f, 8316.090820f, 21455.267578f },
			{ -47523.886719f, 9904.458008f, 21574.015625f }
		});
		Command *f14_to_f15 = new Path(std::vector<coord::Coord> {
			{ -47515.808594f, 11563.567383f, 21760.125000f },
			{ -47279.910156f, 11748.429688f, 21762.779297f },
			{ -46666.496094f, 11652.148438f, 21799.837891f },
			{ -45883.234375f, 10836.951172f, 22352.062500f },
			{ -45862.324219f, 9245.889648f, 22367.404297f },
			{ -46693.566406f, 8448.970703f, 22917.087891f },
			{ -47562.933594f, 8315.893555f, 22955.238281f },
			{ -47522.066406f, 9914.714844f, 23074.005859f }
		});
		
		
		std::vector<Command *> script = {
			mushin_to_room,
			room_to_f13_1,
			buy_entrance_ticket,
			room_to_f13_2,
			f13_to_fujin,
			kill,
			loot,
			fujin_to_f14,
			kill,
			loot,
			f14_to_f15,
			kill,
			loot
		};

		size_t i = 0;
		while (bot::GetState() == bot::Running) {
			
			

			current_player = bns_instance->GetPlayerAddress();
			if (current_player != last_player) {
				last_player = current_player;
				if (last_player != NULL) {
					float *coords = (float *)(current_player + 0x80);
					printf("player = %p (%f, %f, %f)\n", current_player, coords[0], coords[1], coords[2]);
				}
			}
			if (GetAsyncKeyState(VK_NUMPAD0)) {
				while (i < script.size() && bot::GetState() == bot::Running) {
					script[i]->Execute();
					i++;
				}
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_NUMPAD1)) {
				f13_to_fujin->Execute();
			}
			if (GetAsyncKeyState(VK_NUMPAD2)) {
				bns_instance->SendPacketEasy((void *)packets::buy_orb);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_NUMPAD3)) {
				bns_instance->SendTab(bns_instance->GetKeybdDevice());
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
