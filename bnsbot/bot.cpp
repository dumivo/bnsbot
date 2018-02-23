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
#include "dinput.h"

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

void NaksunHallScript(std::vector<std::shared_ptr<bot::Command>>& script) {
	using namespace bot;
	// Set up addresses like player and keybd_device
	script.push_back(std::shared_ptr<Command>(new MushinInstancePrologue()));
	// Go to mushin hall 2
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -53752.6f, 9084.30f, -9406.71f },
		{ -53260.7f, 9625.15f, -9112.16f }
	}, true)));
	// Loading screen
	script.push_back(std::shared_ptr<Command>(new Wait(20000)));
	// Go to in front of the portal
	// Set up addresses like player and keybd_device
	script.push_back(std::shared_ptr<Command>(new MushinInstancePrologue()));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -49862.242188f, 4815.891113f, -8794.495117f },
	})));
}

// Returns a script starting from f13 spawn point until end.
void NaksunScriptF13(std::vector<std::shared_ptr<bot::Command>>& script) {
	using namespace bot;
	// Go to wheel
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -48146.796875f, 9886.035156f, 26070.945313f },
		{ -48472.179688f, 9986.939453f, 26071.937500f },
	})));
	// Buy Orb
	script.push_back(std::shared_ptr<Command>(new BuyTicket()));
	// Go to f13
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47536.691406f, 9676.743164f, 26073.654297f },
		{ -47524.550781f, 9994.874023f, 26074.398438f },
		{ -47536.691406f, 9676.743164f, 26073.654297f },
		{ -47518.839844f, 9890.558594f, 26075.875000f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f14
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47518.402344f, 11749.320313f, 26263.412109f },
		{ -46813.722656f, 11750.712891f, 26263.412109f },
		{ -45744.453125f, 10679.685547f, 26857.095703f },
		{ -45813.156250f, 9360.359375f, 26857.095703f },
		{ -46884.437500f, 8316.925781f, 27460.701172f },
		{ -47529.042969f, 8317.372070f, 27460.701172f },
		{ -47543.992188f, 9896.345703f, 27574.783203f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f15
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47511.640625f, 11761.174805f, 27765.412109f },
		{ -46817.300781f, 11717.775391f, 27765.412109f },
		{ -45768.687500f, 10682.803711f, 28359.097656f },
		{ -45838.808594f, 9361.602539f, 28359.097656f },
		{ -46949.464844f, 8285.130859f, 28964.701172f },
		{ -47522.320313f, 8377.159180f, 28964.701172f },
		{ -47525.453125f, 9987.574219f, 29079.414063f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));

	// Go to exit portal
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47092.3f, 10039.4f, 29080.7f },
		{ -46887.5f, 10038.7f, 29078.8f }
	}, 3))); // Use 3 to tab on the way to the portal (because we are stuck spamming F from earlier..)

	// Use portal
	script.push_back(std::shared_ptr<Command>(new UsePortalF()));

	// Loading screen exit
	script.push_back(std::shared_ptr<Command>(new Wait(30000)));
}

// Returns a script starting from mushin spawn to f13-15 until end.
void NaksunScript(std::vector<std::shared_ptr<bot::Command>>& script) {
	printf("Naksun F13\n");
	using namespace bot;
	NaksunHallScript(script);

	// Enter Dungeon using F13 ticket
	
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -49634.167969f, 4807.551270f, -8795.016602f },
	}, true)));
	script.push_back(std::shared_ptr<Command>(new BuyEntranceTicket()));
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -49454.195313f, 4812.831543f, -8795.212891f }
	}, true)));

	// Sleep loading screen
	script.push_back(std::shared_ptr<Command>(new Wait(20000)));

	// Set up addresses like player and keybd_device
	script.push_back(std::shared_ptr<Command>(new MushinInstancePrologue()));

	NaksunScriptF13(script);
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
		{ -47512.183594f, 11227.820313f, 26154.533203f },
		{ -47512.214844f, 11755.634766f, 26262.564453f },
		{ -46888.621094f, 11678.717773f, 26260.691406f },
		{ -45897.671875f, 10819.937500f, 26848.744141f },
		{ -45817.554688f, 9356.798828f, 26854.378906f },
		{ -46699.261719f, 8454.794922f, 27416.244141f },
		{ -47165.992188f, 8316.100586f, 27456.876953f }, // <-- cutscene
	})));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47242.042969f, 8318.500000f, 27459.689453f },
		{ -47486.191406f, 8318.247070f, 27459.468750f },
		{ -47518.476563f, 8949.526367f, 27606.978516f },
		{ -47527.023438f, 9890.327148f, 27575.427734f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// Go to f15
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47513.117188f, 11300.231445f, 27662.466797f },
		{ -47207.460938f, 11834.181641f, 27763.935547f },
		{ -46798.761719f, 11756.212891f, 27763.099609f },
		{ -45879.492188f, 10841.348633f, 28351.408203f },
		{ -45820.816406f, 9412.381836f, 28356.251953f },
		{ -46692.812500f, 8493.436523f, 28904.669922f },
		{ -46932.953125f, 8362.189453f, 28962.806641f },
		{ -47109.933594f, 8358.036133f, 28963.238281f },
	})));
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47246.890625f, 8356.598633f, 28963.287109f },
		{ -47496.128906f, 8366.648438f, 28962.878906f },
		{ -47522.695313f, 8870.359375f, 29111.226563f },
		{ -47517.484375f, 9952.857422f, 29079.062500f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));

	// Go to exit portal
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47092.3f, 10039.4f, 29080.7f },
		{ -46887.5f, 10038.7f, 29078.8f }
	}, 3))); // Use 3 to tab on the way to the portal (because we are stuck spamming F from earlier..)

	// Use portal
	script.push_back(std::shared_ptr<Command>(new UsePortalF()));

	// Loading screen exit
	script.push_back(std::shared_ptr<Command>(new Wait(30000)));

	return script;
}

// Returns a script starting from mushin spawn to f9-15 until end.
void NaksunF9Script(std::vector<std::shared_ptr<bot::Command>>& script) {
	printf("Naksun F9\n");
	using namespace bot;
	NaksunHallScript(script);

	// Enter Dungeon using F9 ticket
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -49634.167969f, 4807.551270f, -8795.016602f },
	}, true)));
	script.push_back(std::shared_ptr<Command>(new BuyEntranceTicketF9()));
	script.push_back(std::shared_ptr<Command>(new LoadingPath(std::vector<coord::Coord> {
		{ -49454.195313f, 4812.831543f, -8795.212891f }
	}, true)));
	// Sleep loading screen
	script.push_back(std::shared_ptr<Command>(new Wait(20000)));
	// Set up keybd_device
	script.push_back(std::shared_ptr<Command>(new MushinInstancePrologue()));
	// F9 move to boss
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47521.136719f, 9795.482422f, 20080.558594f },
		{ -47516.667969f, 9635.031250f, 20081.855469f },
		{ -47522.000000f, 9869.114258f, 20081.685547f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// F10 move to boss
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47520.843750f, 11711.344727f, 20271.412109f },
		{ -46836.773438f, 11762.383789f, 20271.412109f },
		{ -45784.996094f, 10736.904297f, 20865.095703f},
		{ -45815.214844f, 9411.662109f, 20865.095703f },
		{ -46893.980469f, 8283.570313f, 21459.611328f },
		{ -47472.675781f, 8298.936523f, 21459.611328f },
		{ -47502.828125f, 9928.534180f, 21573.783203f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// F11 move to boss
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47522.773438f, 11728.661133f, 21764.412109f },
		{ -46884.175781f, 11721.245117f, 21764.412109f },
		{ -45770.429688f, 10637.527344f, 22358.101563f },
		{ -45831.906250f, 9450.133789f, 22358.101563f },
		{ -46934.234375f, 8260.546875f, 22959.701172f },
		{ -47476.792969f, 8376.646484f, 22959.701172f },
		{ -47529.914063f, 9948.291016f, 23073.783203f },
	})));
	// Kill
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// F12 move to boss
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47532.539063f, 11732.333008f, 23264.412109f},
		{ -46804.648438f, 11735.137695f, 23264.412109f },
		{ -45758.609375f, 10695.980469f, 23858.103516f },
		{ -45814.511719f, 9390.843750f, 23858.101563f },
		{ -46911.765625f, 8279.994141f, 24458.701172f },
		{ -47519.621094f, 8396.813477f, 24458.701172f },
		{ -47518.968750f, 10032.772461f, 24572.783203f},
	})));
	// KillSnek
	script.push_back(std::shared_ptr<Command>(new CombatSnek()));
	// F12 positioning to boss
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47520.257813f, 9583.652344f, 24570.726563f },
		{ -47519.308594f, 9848.969727f, 24572.404297f },
	})));
	// KillBigSnek
	script.push_back(std::shared_ptr<Command>(new CombatSpin()));
	// F12 Move to loot
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47510.699219f, 9990.306641f, 24573.238281f },
	})));
	// Loot
	script.push_back(std::shared_ptr<Command>(new Loot()));
	// F13 move to boss
	script.push_back(std::shared_ptr<Command>(new Path(std::vector<coord::Coord> {
		{ -47521.730469f, 11744.547852f, 24763.412109f},
		{ -46791.832031f, 11742.782227f, 24763.412109f },
		{ -45762.117188f, 10651.275391f, 25357.097656f},
		{ -45815.722656f, 9348.145508f, 25357.099609f },
		{ -46900.500000f, 8277.464844f, 25958.701172f },
		{ -47517.648438f, 8415.229492f, 25958.701172f },
		{ -47526.097656f, 9493.048828f, 26072.783203f },
	})));

	NaksunScriptF13(script);
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
	bns_instance->SkipCutscene(true);
	bns_instance->RefreshPlayerAddress();
	// Using WinAPI thread because bns doesn't like C++11.:(
	// But somehow we can use std::lock_guard :thinking:.
	// Anyways, just don't use std::thread.
	ChangeState(bot::Ready);
	HANDLE controller_thread =
		CreateThread(0, 0, bot::ControllerThread, NULL, 0, 0);

	uintptr_t last_player = 0;
	uintptr_t current_player = 0;
	const int f9_every_f13 = 8;
	const bool mouse_spam = false;
	int f13_counter = 0;
	while (bot::GetState() != bot::Off) {

		if (GetAsyncKeyState(VK_NUMPAD0)) {
			auto coord = bns_instance->GetPlayerCoord();
			printf("{ %f, %f, %f },\n", coord.x, coord.y, coord.z);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_NUMPAD1)) {
			bns_instance->RefreshPlayerAddress();
			Sleep(500);
		}

		while (bot::GetState() == bot::Running) {
			clock_t timer = clock();
			size_t i = 0;
			std::vector<std::shared_ptr<bot::Command>> script;
			// Mouse spam?
			if (mouse_spam) {
				printf("Using mouse spam\n");
				
				script.push_back(std::shared_ptr<bot::Command>(new MouseSpam()));
				printf("size %u\n", script.size());
			}

			// Do f9 every four f13 runs
			printf("Deciding Script..\n");
			if (f9_every_f13 == f13_counter) {
				f13_counter = 0;
				printf("Deciding F9\n");
				NaksunF9Script(script);
			}
			else {
				printf("Deciding F13\n");
				NaksunScript(script);
			}
			while (i < script.size() && bot::GetState() == bot::Running) {
				script[i]->Execute();
				i++;
				Sleep(500);
			}

			script.clear();
			printf("[BOT] Naksun run done in %i seconds.\n", (clock() - timer) / CLOCKS_PER_SEC);
			Sleep(2500);
			f13_counter++;
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
		Sleep(50);
	}

	hook::RemoveHooks();
	ChangeState(Off);
	printf("[BOT] Waiting for Controller to quit.\n");
	WaitForSingleObject(controller_thread, INFINITE);
	CloseHandle(controller_thread);
	Sleep(1000);
	printf("[BOT] Quitting.\n");
}
