#include "buy_entrance_ticket.h"
#include "bns.h"
#include "packets.h"

bot::BuyEntranceTicket::BuyEntranceTicket() {
}


bool bot::BuyEntranceTicket::Execute() {
	using namespace bns;
	Bns *bns = Bns::getInstance();
	
	// Alright, so if you want to enter mushin's tower with packet sending, you have to point to
	// the inventory ID which holds the specific ticket for that.
	// So we have to.. uh.. recreate this structure in order to do this cleanly.
	// I tried researching where this structure gets build bo no avail :(
	char inventory[] =
	{
		0x70, 0x76, 0x23, 0x41, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x01, 0x00, 0xBD, 0xF7, 0x00, 0x00, 0xA9, 0x4D, 0x75, 0xD7, 0x00, 0x71, 0x00, 0x89 // <-- First Byte = inventory slot id
	};
	char mushin[0x80];
	memcpy(mushin, packets::mushin_f13, 0x80);
	*(uintptr_t *)(mushin + 0x30) = (uintptr_t)inventory;
	*(uintptr_t *)(mushin + 0x38) = (uintptr_t)inventory;
	bns->SendPacketEasy((void *)mushin);
	Sleep(100);
	return false;
}

bot::BuyEntranceTicketF9::BuyEntranceTicketF9() {
}


bool bot::BuyEntranceTicketF9::Execute() {
	using namespace bns;
	Bns *bns = Bns::getInstance();
	bns->SendPacketEasy((void *)packets::mushin_f9);
	Sleep(100);
	return false;
}
