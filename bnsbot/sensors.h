#pragma once

#include "bns.h"
// "Sensors" because they react to specific events in the "environment" aka. the game.
// These are implemented as hooks. So the following functions will get called automatically.
namespace hook {
	static bns::sigs::UpdateTargetHP oUpdateTargetHP;
	static bns::sigs::UpdateKeybdDevice oUpdateKeybdDevice;
	static bns::sigs::InventoryEvent oInventoryEvent;
	static bns::sigs::SendPacket oSendPacket;
	static bns::sigs::Move oMove;

	void UpdateTargetHP(uintptr_t rcx, unsigned long hp, uintptr_t r8);
	void *UpdateKeybdDevice(uintptr_t rcx, uintptr_t rdx);
	void *InventoryEvent(uintptr_t intenvory_slot, unsigned long slot_id, int r8);
	void *SendPacket(uintptr_t rcx, uintptr_t rdx, void *data);
	bool *Move(uintptr_t player, float x, float y, float z);

	bool SetupHooks();
	void RemoveHooks();
}

