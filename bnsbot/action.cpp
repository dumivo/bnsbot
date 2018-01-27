#include "action.h"
#include "bns.h"
#include "bot.h"
#include <ctime>

using namespace bns;
void SendPacket(int a, int b) {
	using namespace bns;
	
	
}

void bot::X() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyboardEasy(1, 5);
}

void bot::Z() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyboardEasy(1, 4);
	bot::cooldown_start_time = clock();
}

void bot::V() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyboardEasy(1, 7);
}

void bot::F() {
	Bns *bns = Bns::getInstance();
	bns->SendActionEasy(1, 2);
}

void bot::Q() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyboardEasy(1, 5);
}

void bot::UIF() {
	Bns *bns = Bns::getInstance();
	bns->SendActionEasy(0x11, 2);
}

void bot::PortalF() {
	Bns *bns = Bns::getInstance();
	bns->SendActionEasy(0x13, 2);
}

void bot::PickAllF() {
	Bns *bns = Bns::getInstance();
	bns->SendActionEasy(0x14, 2);
}

void bot::Tab() {
	Bns *bns = Bns::getInstance();
	bns->SendTabEasy();
}

void bot::Esc() {
	Bns *bns = Bns::getInstance();
	bns->SendEscEasy();
}
