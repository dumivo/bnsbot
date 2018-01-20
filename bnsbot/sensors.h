#pragma once

#include "bns.h"
// "Sensors" because they react to specific events in the "environment" aka. the game.
// These are implemented as hooks. So the following functions will get called automatically.

typedef void(__fastcall *UpdateTargetHP)(uintptr_t rcx, unsigned long hp, uintptr_t r8);
typedef void *(__fastcall *UpdateKeybdDevice)(uintptr_t rcx, uintptr_t rdx, uintptr_t r8);
typedef void *(__fastcall *InventoryEvent)(uintptr_t intenvory_slot, unsigned long slot_id, int r8);

namespace hook {
	bns::sigs::UpdateTargetHP oUpdateTargetHP;
	bns::sigs::UpdateKeybdDevice oUpdateKeybdDevice;
	bns::sigs::InventoryEvent oInventoryEvent;

	void UpdateTargetHP(uintptr_t rcx, unsigned long hp, uintptr_t r8);
	void *UpdateKeybdDevice(uintptr_t rcx, uintptr_t rdx, uintptr_t r8);
	void *InventoryEvent(uintptr_t intenvory_slot, unsigned long slot_id, int r8);

	bool SetupHooks();
}

