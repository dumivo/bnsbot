#include "loot.h"
#include "bns.h"
#include "action.h"
#include <ctime>

bot::Loot::Loot() {
}


bool bot::Loot::Execute() {
	// These sleep timers are really generous here because looting is like the
	// most important aspect of this program.
	Sleep(1000);
	UIF();
	Sleep(1000);
	clock_t start_time = clock();
	double seconds_passed = 0;
	while (seconds_passed <= 2) {
		PickAllF();
		seconds_passed = (clock() - start_time) / CLOCKS_PER_SEC;
		Sleep(200);
	}
	
	Sleep(1000);
	return false;
}
