#include "loot.h"
#include "bns.h"
#include "action.h"

bot::Loot::Loot() {
}

bot::Loot::~Loot() {
}

bool bot::Loot::Execute() {
	Sleep(500);
	UIF();
	Sleep(500);
	PickAllF();
	return false;
}
