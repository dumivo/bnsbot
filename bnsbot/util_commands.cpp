#include <Windows.h>
#include "util_commands.h"
#include "action.h"
#include "bns.h"
#include <ctime>

bot::Wait::Wait() {
}

bot::Wait::Wait(unsigned int time_to_wait_ms) {
	time_to_wait_ms_ = time_to_wait_ms;
}


bool bot::Wait::Execute() {
	using namespace bns;
	Bns *bns = Bns::getInstance();
	bns->SetSleep(true);
	printf("Sleeping for %ums\n", time_to_wait_ms_);
	Sleep(time_to_wait_ms_);
	printf("Wake up\n");
	bns->SetSleep(false);
	return false;
}

bot::UseF::UseF() {
}


bool bot::UseF::Execute() {
	Sleep(1000);
	UIF();
	return false;
}

bot::WaitLoadingScreen::WaitLoadingScreen() {
}


bool bot::WaitLoadingScreen::Execute() {
	using namespace bns;
	// Wait until player address changes to a non-zero one.
	// That way we make sure that we don't access a bad address.
	Bns *bns = Bns::getInstance();
	uintptr_t player_old = bns->GetPlayerAddress();
	uintptr_t player = 0;
	printf("[WAIT] Waiting for loading screen..\n");
	Sleep(30000);
	clock_t timer = clock();
	double seconds_passed = 0;
	do {
		// It's better to wait too long than too short.
		Sleep(5000);
		player = bns->GetPlayerAddress();
		seconds_passed = (clock() - timer) / CLOCKS_PER_SEC;
	} while ((player == player_old || !player) && seconds_passed <= 120);
	printf("[WAIT] Loading screen finished\n");

	Sleep(5000);

	return false;
}

bot::UsePortalF::UsePortalF() {
}


bool bot::UsePortalF::Execute() {
	Sleep(1000);
	PortalF();
	Sleep(500);
	return false;
}

bot::UseEsc::UseEsc() {
}


bool bot::UseEsc::Execute() {
	Esc();
	return false;
}

bot::SkipCutscene::SkipCutscene(bool is_robot) {
	is_robot_ = is_robot;
}

bot::SkipCutscene::SkipCutscene() {
	is_robot_ = true;
}


bool bot::SkipCutscene::Execute() {
	using namespace bns;
	char data[] = {
		0xB0, 0x98, 0x39, 0x41, 0x01, 0x00, 0x00, 0x00, 0x10, 0xE0, 0x5C, 0x0A, 0x01, 0x00, 0x00, 0x00,
		0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 83 robot
		0x49, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x57, 0x00, 0x30, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00 // 38 robot
	};
	Bns *bns = Bns::getInstance();
	Sleep(1000);
	printf("[EXIT_CUTSCENE] hm.. let's escape this bullshit\n");
	if (!is_robot_) {
		data[0x18] = 0x84;
		data[0x2A] = 0x39;
	}
	bns->ExitLoadingScreen((uintptr_t)data);
	Sleep(1000);
	return false;
}

bool bot::MouseSpam::Execute() {
	printf("Doing some mouse thing bullshit\n");
	using namespace bns;
	Bns *bns = Bns::getInstance();
	HWND foreground_window = GetForegroundWindow();
	RECT rect;
	GetWindowRect(foreground_window, &rect);
	SetCursorPos(rect.left + 0, rect.top + 0);
	Sleep(500);
	SetCursorPos(rect.left + 100, rect.top + 100);
	Sleep(500);
	SetCursorPos(rect.left + 200, rect.top + 200);
	Sleep(500);
	return false;
}

bool bot::MushinInstancePrologue::Execute() {
	auto *bns = bns::Bns::getInstance();
	printf("Refreshing addresses\n");
	bns->RefreshPlayerAddress();
	bns->RefreshKeybdDevice();
	return false;
}
