#include "sensors.h"

void hook::UpdateTargetHP(uintptr_t rcx, unsigned long hp, uintptr_t r8) {
	// TODO: Notify when hp reaches specific thresholds.
}

void * hook::UpdateKeybdDevice(uintptr_t rcx, uintptr_t rdx, uintptr_t r8) {
	// TODO: save rcx or continously update rcx..
	return nullptr;
}

void * hook::InventoryEvent(uintptr_t intenvory_slot, unsigned long slot_id, int r8) {
	// TODO: Log and identify looted items..
	return nullptr;
}

bool hook::SetupHooks() {
	return false;
}
