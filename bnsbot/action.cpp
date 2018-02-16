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
	bns->SendKeyEasyOnce(bnskey::X);
}

void bot::Z() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::Z);
	bns->SetCooldownStartTime();
}

void bot::V() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::V);
}

void bot::F() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::F);
}

void bot::UIF() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::F);
}

void bot::One() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::One);
}

void bot::Three() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::Three);
}

void bot::PortalF() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::F);
}

void bot::PickAllF() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::F);
}

// Actually presses "7".
void bot::Nothing() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(0x37);
}

void bot::LMB() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::LMB);
}

void bot::Tab() {
	Bns *bns = Bns::getInstance();
	bns->SendKeyEasyOnce(bnskey::Tab);
}
