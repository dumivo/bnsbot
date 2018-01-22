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
	double milliseconds_passed = (clock() - bot::cooldown_start_time) / (CLOCKS_PER_SEC / 1000);
	while (milliseconds_passed <= 45000) {
		Sleep(100);
	}

	while (bot::GetState() != bot::Running) {
		Sleep(100);
	}

	// X -> Z -> F -> F -> F .. -> F until HP <= 1
	bns::Bns *bns_instance = bns::Bns::getInstance();
	while (bns_instance->GetTargetHP() == 0) {
		Sleep(50);
	}

	// Daze the enemy and wait until the daze hit for frame perfect animation cancelling.
	unsigned long hp = bns_instance->GetTargetHP();
	V();
	while (bns_instance->GetTargetHP() >= hp) {
	}

	X();
	Sleep(500);
	Z();
	Sleep(250);

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F..\n");
#endif

	clock_t start_time = clock();
	
	bns_instance->SetTargetDead(false);
	// Spam the shit out of the monster until it's dead.
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

	return false;
}

bot::CombatSpin::CombatSpin() {
}

bot::CombatSpin::~CombatSpin() {
}

bool bot::CombatSpin::Execute() {
	Combat::Execute();
#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F done.\nSpamming Tab..\n");
#endif
	bns::Bns *bns_instance = bns::Bns::getInstance();
	// Spam spin because asshat Naksun has to speak at 1hp.
	clock_t start_time = clock();
	double milliseconds_passed;
	while (bns_instance->GetTargetHP() >= 1) {
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		if (milliseconds_passed >= 50) {
			Tab();
			milliseconds_passed = 0;
		}
	}

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming Tab done.\n");
#endif
	return false;
}
