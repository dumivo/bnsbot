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
	auto *bns = bns::Bns::getInstance();
	printf("[UsePortalF] Waiting for Z cd..\n");
	/*while ((clock() - bns->GetCooldownStartTime()) / CLOCKS_PER_SEC < 45) {

	}*/
	printf("[UsePortalF] Z up\n");
	PortalF();
	Sleep(500);
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
