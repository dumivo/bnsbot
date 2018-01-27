#include <Windows.h>
#include <vector>
#include <ctime>

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
#include "util_commands.h"

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

std::vector<std::shared_ptr<bot::Command>> NaksunScript() {
	using namespace bot;
	// Set up script
	{
		/*Command *mushin_to_room = new LoadingPath(std::vector<coord::Coord> {
			{ -53739.0f, 6367.32f, -9611.62f },
			{ -53752.6f, 9084.30f, -9406.71f },
			{ -53260.7f, 9625.15f, -9112.16f }
		});
		Command *room_to_f13_1 = new Path(std::vector<coord::Coord> {
			{ -49862.242188f, 4815.891113f, -8794.495117f },
			{ -49695.867188f, 4560.463379f, -8795.165039f }
		});
		Command *buy_entrance_ticket = new BuyEntranceTicket();
		Command *room_to_f13_2 = new LoadingPath(std::vector<coord::Coord> {
			{ -49634.167969f, 4807.551270f, -8795.016602f },
			{ -49454.195313f, 4812.831543f, -8795.212891f }
		});
		Command *f13_to_fujin = new Path(std::vector<coord::Coord> {
			{ -47512.339844f, 9216.077148f, 26070.933594f },
			{ -47950.460938f, 9750.791016f, 26070.445313f }
		});
		Command *go_to_wheel = new Path(std::vector<coord::Coord> {
			{-48366.7f, 10034.2f, 26071.8f},
			{ -48472.2f, 10050.0f, 26072.6f }
		});
		Command *buy_orb = new BuyTicket();
		Command *back_to_fujin = new Path(std::vector<coord::Coord> {
			{ -47520.6f, 9702.38f, 26073.5f },
			{ -47521.0f, 9828.79f, 26075.8f },
			{ -47512.339844f, 9216.077148f, 26070.933594f },
			{ -47521.0f, 9828.79f, 26075.8f },
		});
		Command *kill = new CombatSpin();
		Command *loot = new Loot();
		Command *fujin_to_f14 = new Path(std::vector<coord::Coord> {
			{ -47520.2f, 10238.0f, 26073.6f },
			{ -47521.2f, 11562.5f, 26250.5f },
			{ -47533.0f, 11763.3f, 26262.4f },
			{ -46799.5f, 11717.4f, 26260.2f },
			{ -45906.7f, 10856.1f, 26835.7f },
			{ -45849.2f, 9300.72f, 26854.2f },
			{ -46723.6f,8434.4f, 27431.9f },
			{ -47487.7f, 8367.78f, 27456.1f },
			{ -47503.9f, 8750.65f, 27595.1f },
			{ -47522.4f, 9857.96f, 27575.1f }
		});
		Command *f14_to_f15 = new Path(std::vector<coord::Coord> {
			{ -47519.4f, 11570.7f, 27762.0f },
			{ -47530.3f, 11761.3f ,27765.1f },
			{ -46739.3f, 11706.1f, 27771.1f },
			{ -45865.0f, 10792.0f, 28359.4f },
			{ -45860.7f, 9221.1f, 28374.2f },
			{ -46690.0f, 8445.4f, 28919.7f },
			{ -47579.5f, 8317.82f, 28960.2f },
			{ -47520.2f, 8750.64f, 29099.2f },
			{ -47522.5f, 9963.7f, 29078.8f }
		});
		Command *f15_to_exit = new Path(std::vector<coord::Coord> {
			{ -47092.3f, 10039.4f, 29080.7f },
			{ -46887.5f, 10038.7f, 29078.8f }
		});
		Command *portal_f = new UsePortalF();
		Command *exit = new WaitLoadingScreen();*/
	}

	std::vector<std::shared_ptr<Command>> script;
	// Go to mushin hall 2
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -53739.0f, 6367.32f, -9611.62f },
		{ -53752.6f, 9084.30f, -9406.71f },
		{ -53260.7f, 9625.15f, -9112.16f }
	})));
	// Buy entrance ticket and go to f13
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -49862.242188f, 4815.891113f, -8794.495117f },
		{ -49695.867188f, 4560.463379f, -8795.165039f }
	})));
	script.push_back(std::shared_ptr<Command>(new BuyEntranceTicket()));
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -49634.167969f, 4807.551270f, -8795.016602f },
		{ -49454.195313f, 4812.831543f, -8795.212891f }
	})));
	// Go to wheel
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47512.339844f, 9216.077148f, 26070.933594f },
		{ -47950.460938f, 9750.791016f, 26070.445313f }
	})));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{-48366.7f, 10034.2f, 26071.8f},
		{ -48472.2f, 10050.0f, 26072.6f }
	})));
	// Buy Orb
	script.push_back(std::shared_ptr<Command>(new BuyTicket()));
	// Go to f13
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47520.6f, 9702.38f, 26073.5f },
		{ -47521.0f, 9828.79f, 26075.8f },
		{ -47512.339844f, 9216.077148f, 26070.933594f },
		{ -47521.0f, 9828.79f, 26075.8f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f14
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47520.2f, 10238.0f, 26073.6f },
		{ -47521.2f, 11562.5f, 26250.5f },
		{ -47533.0f, 11763.3f, 26262.4f },
		{ -46799.5f, 11717.4f, 26260.2f },
		{ -45906.7f, 10856.1f, 26835.7f },
		{ -45849.2f, 9300.72f, 26854.2f },
		{ -46723.6f, 8434.4f, 27431.9f },
		{ -47163.250000f, 8269.625977f, 27458.460938f }, // <- cutscene
	}, 1)));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47487.7f, 8367.78f, 27456.1f },
		{ -47503.9f, 8750.65f, 27595.1f },
		{ -47522.4f, 9857.96f, 27575.1f }
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f15
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47512.441406f, 11151.376953f, 27650.451172f },
		{ -47508.113281f, 11562.530273f, 27758.064453f },
		{ -47327.945313f, 11814.504883f, 27763.818359f },
		{ -46772.894531f, 11759.719727f, 27763.025391f },
		{ -45887.484375f, 10878.573242f, 28336.445313f },
		{ -45787.949219f, 9385.083984f, 28356.222656f },
		{ -46726.328125f, 8437.148438f, 28932.248047f },
		{ -47165.304688f, 8357.212891f, 28961.007813f },
		{ -47237.433594f, 8340.607422f, 28962.865234f },
	}, 2)));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47496.968750f, 8301.460938f, 28962.748047f },
		{ -47503.562500f, 8721.144531f, 29090.574219f },
		{ -47528.238281f, 9922.763672f, 29078.820313f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));

	// Go to exit portal
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47092.3f, 10039.4f, 29080.7f },
		{ -46887.5f, 10038.7f, 29078.8f }
	})));

	// Use portal
	script.push_back(std::shared_ptr<Command>(new UsePortalF()));

	// Loading screen exit
	script.push_back(std::shared_ptr<Command>(new WaitLoadingScreen()));

	return script;
}

std::vector<std::shared_ptr<bot::Command>> NaksunScript2() {
	using namespace bot;
	std::vector<std::shared_ptr<Command>> script;
	// Go to mushin hall 2
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -53739.0f, 6367.32f, -9611.62f },
		{ -53752.6f, 9084.30f, -9406.71f },
		{ -53260.7f, 9625.15f, -9112.16f }
	}, true)));
	script.push_back(std::shared_ptr<Command>(new Wait(20000)));
	// Buy entrance ticket and go to f13
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -49862.242188f, 4815.891113f, -8794.495117f },
		{ -49695.867188f, 4560.463379f, -8795.165039f }
	})));
	script.push_back(std::shared_ptr<Command>(new BuyEntranceTicket()));
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -49634.167969f, 4807.551270f, -8795.016602f },
		{ -49454.195313f, 4812.831543f, -8795.212891f }
	}, true)));
	script.push_back(std::shared_ptr<Command>(new Wait(20000)));
	// Go to wheel
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47512.339844f, 9216.077148f, 26070.933594f },
		{ -47950.460938f, 9750.791016f, 26070.445313f }
	})));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{-48366.7f, 10034.2f, 26071.8f},
		{ -48472.2f, 10050.0f, 26072.6f }
	})));
	// Buy Orb
	script.push_back(std::shared_ptr<Command>(new BuyTicket()));
	// Go to f13
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47520.6f, 9702.38f, 26073.5f },
		{ -47521.0f, 9828.79f, 26075.8f },
		{ -47512.339844f, 9216.077148f, 26070.933594f },
		{ -47521.0f, 9828.79f, 26075.8f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f14
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47520.2f, 10238.0f, 26073.6f },
		{ -47521.2f, 11562.5f, 26250.5f },
		{ -47533.0f, 11763.3f, 26262.4f },
		{ -46799.5f, 11717.4f, 26260.2f },
		{ -45906.7f, 10856.1f, 26835.7f },
		{ -45849.2f, 9300.72f, 26854.2f },
		{ -46723.6f, 8434.4f, 27431.9f },
		{ -47163.250000f, 8269.625977f, 27458.460938f }, // <- cutscene
	}, 1)));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47487.7f, 8367.78f, 27456.1f },
		{ -47503.9f, 8750.65f, 27595.1f },
		{ -47522.4f, 9857.96f, 27575.1f }
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f15
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47512.441406f, 11151.376953f, 27650.451172f },
		{ -47508.113281f, 11562.530273f, 27758.064453f },
		{ -47327.945313f, 11814.504883f, 27763.818359f },
		{ -46772.894531f, 11759.719727f, 27763.025391f },
		{ -45887.484375f, 10878.573242f, 28336.445313f },
		{ -45787.949219f, 9385.083984f, 28356.222656f },
		{ -46726.328125f, 8437.148438f, 28932.248047f },
		{ -47165.304688f, 8357.212891f, 28961.007813f },
		{ -47237.433594f, 8340.607422f, 28962.865234f },
	}, 2)));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47496.968750f, 8301.460938f, 28962.748047f },
		{ -47503.562500f, 8721.144531f, 29090.574219f },
		{ -47528.238281f, 9922.763672f, 29078.820313f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));

	// Go to exit portal
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47092.3f, 10039.4f, 29080.7f },
		{ -46887.5f, 10038.7f, 29078.8f }
	})));

	// Use portal
	script.push_back(std::shared_ptr<Command>(new UsePortalF()));

	// Loading screen exit
	script.push_back(std::shared_ptr<Command>(new Wait(30000)));

	return script;
}

std::vector<std::shared_ptr<bot::Command>> TestScript() {
	using namespace bot;

	std::vector<std::shared_ptr<Command>> script;
	// Go to mushin hall 2
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -50203.398438f, 5197.004395f, -8793.618164f },
		{ -50351.117188f, 5253.174316f, -8793.519531f },
		{ -50557.125000f, 5073.280273f, -8795.225586f },
		{ -50679.828125f, 4795.179199f, -8795.210938f },
		{ -50712.726563f, 4511.964844f, -8795.152344f },
		{ -50546.304688f, 4395.892090f, -8795.036133f },
		{ -50200.500000f, 4346.148438f, -8795.317383f },
		{ -49948.027344f, 4378.123535f, -8794.969727f },
		{ -49711.273438f, 4850.812500f, -8795.113281f },
		{ -49852.253906f, 5070.373047f, -8795.143555f },
		{ -50090.820313f, 5197.945801f, -8795.115234f },
		{ -50414.859375f, 5190.105957f, -8795.094727f },
		{ -50609.093750f, 4975.851563f, -8794.959961f },
		{ -50584.875000f, 4678.564453f, -8794.998047f },
		{ -50407.687500f, 4450.421875f, -8794.936523f },
		{ -50204.234375f, 4461.450195f, -8794.732422f },
		{ -50238.777344f, 5089.135742f, -8795.890625f },
		{ -50237.515625f, 5259.410156f, -8793.942383f }
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new Combat()));
	return script;
}

std::vector<std::shared_ptr<bot::Command>> TestScript2() {
	using namespace bot;

	std::vector<std::shared_ptr<Command>> script;
	// Go to mushin hall 2
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -50203.398438f, 5197.004395f, -8793.618164f },
		{ -50351.117188f, 5253.174316f, -8793.519531f },
		{ -50557.125000f, 5073.280273f, -8795.225586f },
		{ -50679.828125f, 4795.179199f, -8795.210938f },
		{ -50712.726563f, 4511.964844f, -8795.152344f },
		{ -50546.304688f, 4395.892090f, -8795.036133f },
		{ -50200.500000f, 4346.148438f, -8795.317383f },
		{ -49948.027344f, 4378.123535f, -8794.969727f },
		{ -49711.273438f, 4850.812500f, -8795.113281f },
		{ -49852.253906f, 5070.373047f, -8795.143555f },
		{ -50090.820313f, 5197.945801f, -8795.115234f },
		{ -50414.859375f, 5190.105957f, -8795.094727f },
		{ -50609.093750f, 4975.851563f, -8794.959961f },
		{ -50584.875000f, 4678.564453f, -8794.998047f },
		{ -50407.687500f, 4450.421875f, -8794.936523f },
		{ -50204.234375f, 4461.450195f, -8794.732422f },
		{ -50238.777344f, 5089.135742f, -8795.890625f },
		{ -50237.515625f, 5259.410156f, -8793.942383f }
	})));
	return script;
}

void FreeScript(std::vector<bot::Command *> script) {
	for (auto element : script) {
		delete element;
	}
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
	while (bot::GetState() != bot::Off) {

		if (GetAsyncKeyState(VK_NUMPAD0)) {
			printf("boom\n");
			while (bot::GetState() != bot::Off) {
				char data[] = 
				{
					0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x7E, 0x49, 0xA9, 0x92, 0x23, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
				};
				bns_instance->SendKey(bns_instance->GetKeybdDevice() - 0x8DD90, data, 0);
			}
		}

		while (bot::GetState() == bot::Running) {
			clock_t timer = clock();
			size_t i = 0;
			std::vector<std::shared_ptr<Command>> script = NaksunScript2();
			while (i < script.size() && bot::GetState() == bot::Running) {
				script[i]->Execute();
				i++;
				Sleep(1000);
			}

			script.clear();
			printf("[BOT] Naksun run done in %i seconds.\n", (clock() - timer) / CLOCKS_PER_SEC);
			Sleep(5000);
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
