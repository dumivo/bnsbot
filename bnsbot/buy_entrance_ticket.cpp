#include "buy_entrance_ticket.h"
#include "bns.h"
#include "packets.h"

bot::BuyEntranceTicket::BuyEntranceTicket() {
}

bot::BuyEntranceTicket::~BuyEntranceTicket() {
}

bool bot::BuyEntranceTicket::Execute() {
	using namespace bns;
	Bns *bns = Bns::getInstance();
	bns->SendPacketEasy((void *)packets::mushin_f9);
	Sleep(100);
	return false;
}
