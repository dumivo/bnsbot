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
		milliseconds_passed = clock();
		printf("Waiting for cd\n");
	}

	while (bot::GetState() == bot::Suspended) {
		Sleep(1000);
		printf("Sleeping\n");
	}

	// X -> Z -> F -> F -> F .. -> F until HP <= 1
	bns::Bns *bns_instance = bns::Bns::getInstance();
	
	while (bns_instance->GetTargetHP() == 0) {
		Sleep(500);
		printf("Target hp is 0\n");
	}

	printf("Start combat\n");

	// Daze the enemy and wait until the daze hit for frame perfect animation cancelling.
	unsigned long hp = bns_instance->GetTargetHP();
	One();
	// Wait until enemy lost hp, but with a max. timeout
	clock_t start_time = clock();
	milliseconds_passed = 0;
	while (bns_instance->GetTargetHP() >= hp && milliseconds_passed < 1000) {//
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
	}
	Sleep(50);
	Three();
	Sleep(200);
	for (int i = 0; i < 2; i++) {
		X();
		Sleep(200);
	}
	for (int i = 0; i < 2; i++) {
		Z();
		Sleep(100);
	}
	

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F..\n");
#endif

	start_time = clock();
	
	bns_instance->SetTargetDead(false);
	// Spam the shit out of the monster until it's dead.
	while (bns_instance->GetTargetHP() > 1 && GetState() != Suspended) {
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		// Don't spam too much fucker or else the client will completely shut down man..
		if (milliseconds_passed >= 100) {
			F();
			milliseconds_passed = 0;
		}
		hp = bns_instance->GetTargetHP();
	}

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F done.\n");
#endif

	bns_instance->SendKeyEasy(0x0);
	return false;
}

bot::CombatSpin::CombatSpin() {
}

bot::CombatSpin::~CombatSpin() {
}

bool bot::CombatSpin::Execute() {
	Combat::Execute();
	Sleep(100);
#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F done.\nSpamming Tab..\n");
#endif
	bns::Bns *bns_instance = bns::Bns::getInstance();
	bns_instance->SendKeyEasy(0x0);
	// Spam spin because asshat Naksun has to speak at 1hp.
	clock_t start_time = clock();
	double milliseconds_passed;
	while (bns_instance->GetTargetHP() >= 1) {
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		if (milliseconds_passed >= 200) {
			printf("[COMBAT] Tabbing\n");
			Tab();
			milliseconds_passed = 0;
		}
	}

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming Tab done.\n");
#endif
	bns_instance->SendKeyEasy(0x0);
	return false;
}

bot::CombatSnek::CombatSnek() {
}

bot::CombatSnek::~CombatSnek() {
}

bool bot::CombatSnek::Execute() {
	printf("[COMBAT] Alright we are at snek let's hope nothing goes wrong\n");
	Sleep(55000);
	clock_t start_time = clock();
	double seconds_passed = 0;
	bns::Bns *bns_instance = bns::Bns::getInstance();
	while (seconds_passed < 175) {
		bns_instance->SendKeyEasy(bnskey::Tab);
		Sleep(4000);
		bns_instance->SendKeyUpEasy(bnskey::Tab);
		bns_instance->SendKeyEasy(bnskey::LMB);
		Sleep(3000);
		bns_instance->SendKeyUpEasy(bnskey::LMB);
		seconds_passed = (clock() - start_time) / CLOCKS_PER_SEC;
	}
	F();
	printf("Boring phase ended. Waiting for the boss to spawn\n");
	Sleep(30000);
	return false;
}
