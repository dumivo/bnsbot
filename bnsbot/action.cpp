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
	bns->SendKeyEasyOnce(0x58);
}

void bot::Z() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x59);
	bns->SetCooldownStartTime();
}

void bot::V() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x56);
}

void bot::F() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x46);
}

void bot::Q() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyboardEasy(1, 5);
}

void bot::UIF() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x46);
}

void bot::One() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x31);
}

void bot::Three() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x33);
}

void bot::PortalF() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x46);
}

void bot::PickAllF() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x46);
}

// Actually presses "7".
void bot::Nothing() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x37);
}

void bot::LMB() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x52);
}

void bot::Tab() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x09);
}

void bot::Esc() {
	Bns *bns = Bns::getInstance();
	bns->SendEscEasy();
}
