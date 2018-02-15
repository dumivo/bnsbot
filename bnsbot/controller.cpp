#include "bot.h"
#include "controller.h"

DWORD bot::ControllerThread(LPVOID param) {
	while (bot::GetState() != bot::Off) {
		if (GetAsyncKeyState(VK_INSERT)) {
			bot::ChangeState(bot::Running);
		}
		else if (GetAsyncKeyState(VK_DELETE)) {
			bot::ChangeState(bot::Suspended);
		}
		else if (GetAsyncKeyState(VK_END)) {
			bot::ChangeState(bot::Off);
		}
		else if (GetAsyncKeyState(VK_HOME)) {
			printf("Starting in 63s\n");
			Sleep(63000);
			bot::ChangeState(bot::Running);
		}
		Sleep(250);
	}

	printf("[CONTROLLER] Exited.\n");
	return 0;
}