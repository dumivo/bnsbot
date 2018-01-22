#include "combat.h"
#include "bns.h"
#include "action.h"
#include "bot.h"

#include <ctime>

#define COMBAT_SHOW_DEBUG_MESSAGES 0

bot::Combat::Combat() {
}

bot::Combat::~Combat() {
}

bool bot::Combat::Execute() {
	// X -> Z -> F -> F -> F .. -> F until HP <= 1 -> spin until hp <= 0
	bns::Bns *bns_instance = bns::Bns::getInstance();
	while (bns_instance->GetTargetHP() == 0) {
		Sleep(50);
	}
	X();
	Sleep(500);
	Z();
	Sleep(250);

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F..\n");
#endif

	// Spam the shit out of the monster until it's dead.
	unsigned long hp = bns_instance->GetTargetHP();
	clock_t start_time = clock();
	double milliseconds_passed;
	bns_instance->SetTargetDead(false);
	while (!bns_instance->IsTargetDead() && GetState() == Running) {
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		if (milliseconds_passed >= 10) {
			F();
			milliseconds_passed = 0;
		}
		hp = bns_instance->GetTargetHP();
	}

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F done.\n");
#endif
	Sleep(200);
	return false;
}
