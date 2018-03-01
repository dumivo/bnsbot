#include "combat.h"
#include "bns.h"
#include "action.h"
#include "bot.h"

#include <ctime>

#define COMBAT_SHOW_DEBUG_MESSAGES 0

bot::Combat::Combat() {
}


bool bot::Combat::Execute() {
	// X -> Z -> F -> F -> F .. -> F until HP <= 1
	bns::Bns *bns_instance = bns::Bns::getInstance();
	double milliseconds_passed = (clock() - bns_instance->GetCooldownStartTime()) / (CLOCKS_PER_SEC / 1000);
	while (milliseconds_passed <= 45000) {
		Sleep(100);
		milliseconds_passed = clock();
		printf("Waiting for cd\n");
	}

	while (bot::GetState() == bot::Suspended) {
		Sleep(1000);
		printf("Sleeping\n");
	}
	
	while (bns_instance->GetTargetHP() == 0) {
		Sleep(500);
		printf("Target hp is 0\n");
	}

	printf("Start combat\n");

	// Daze the enemy and wait until the daze hit for frame perfect animation cancelling.
	uintptr_t hp_adr = bns_instance->GetTargetHPAddress();
	if (!hp_adr) {
		printf("[COMBAT] HP Address is 0.\n");
		return false;
	}
	else {
		printf("[COMBAT] HP address is %p\n", (void *)hp_adr);
	}
	bns_instance->RefreshKeybdDevice();
	int hp = *(int *)hp_adr;
	printf("Dazing\n");
	V();
	// Wait until enemy lost hp, but with a max. timeout
	clock_t start_time = clock();
	milliseconds_passed = 0;
	while (*(int *)hp_adr >= hp && milliseconds_passed < 1000) {//
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		if (milliseconds_passed >= 1000) {
			printf("Q\n");
			bns_instance->SendKeyEasyOnce(bnskey::Q);
		}
	}
	Sleep(100);
	bns_instance->SendKeyEasy(bnskey::X);
	Sleep(400);
	bns_instance->SendKeyEasy(bnskey::Z);
	Sleep(300);
	

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F..\n");
#endif

	start_time = clock();
	clock_t start_time_timeout = clock();
	bns_instance->SendKeyEasy(bnskey::F);
	// Spam the shit out of the monster until it's dead.
	while (*(int *)hp_adr > 1 && GetState() != Suspended && milliseconds_passed <= 10000) {
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		// Don't spam too much fucker or else the client will completely shut down man..
		F();
		if (milliseconds_passed >= 150) {
			milliseconds_passed = 0;
			start_time = clock();
		}
		Sleep(50);
		milliseconds_passed = (clock() - start_time_timeout) / (CLOCKS_PER_SEC / 1000);
	}
	


#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F done.\n");
#endif
	return false;
}

bot::CombatSpin::CombatSpin() {
}

bool bot::CombatSpin::Execute() {
	Combat::Execute();
	Sleep(100);
#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming F done.\nSpamming Tab..\n");
#endif
	bns::Bns *bns_instance = bns::Bns::getInstance();
	uintptr_t hp_adr = bns_instance->GetTargetHPAddress();
	if (!hp_adr) {
		printf("HP is null\n");
		return false;
	}
	// Spam spin because asshat Naksun has to speak at 1hp.
	clock_t start_time = clock();
	double milliseconds_passed;
	while (*(int *)hp_adr >= 1) {
		milliseconds_passed = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
		if (milliseconds_passed >= 200) {
			Tab();
			milliseconds_passed = 0;
		}
		Sleep(50);
	}

#if defined (COMBAT_SHOW_DEBUG_MESSAGES)
	printf("[COMBAT] Spamming Tab done.\n");
#endif
	bns_instance->SendKeyUpEasy(bnskey::Three);
	bns_instance->SendKeyUpEasy(bnskey::X);
	bns_instance->SendKeyUpEasy(bnskey::Z);
	bns_instance->SendKeyUpEasy(bnskey::F);
	return false;
}

bot::CombatSnek::CombatSnek() {
}

bool bot::CombatSnek::Execute() {
	printf("[COMBAT] Alright we are at snek let's hope nothing goes wrong\n");
	Sleep(54000);
	clock_t start_time = clock();
	double seconds_passed = 0;
	bns::Bns *bns_instance = bns::Bns::getInstance();
	while (seconds_passed < 175) {
		clock_t skill_clock = clock();
		while ((clock() - skill_clock) / CLOCKS_PER_SEC < 4) {
			Tab();
			Sleep(500);
		}
		skill_clock = clock();
		while ((clock() - skill_clock) / CLOCKS_PER_SEC < 1) {
			LMB();
			Sleep(200);
		}
		/*bns_instance->SendKeyEasy(bnskey::Tab);
		Sleep(4000);
		bns_instance->SendKeyUpEasy(bnskey::Tab);
		Sleep(100);
		bns_instance->SendKeyEasy(bnskey::LMB);
		Sleep(3000);
		bns_instance->SendKeyUpEasy(bnskey::LMB);*/
		seconds_passed = (clock() - start_time) / CLOCKS_PER_SEC;
	}
	printf("Boring phase ended. Waiting for the boss to spawn\n");
	Sleep(30000);
	return false;
}
