#include "sensors.h"
#include "PolyHook.h"

static std::shared_ptr<PLH::Detour> Detour_UpdateTargetHP(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_UpdateKeybdDevice(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_InventoryEvent(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_SendPacket(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_Move(new PLH::Detour);

static bns::Bns *bns_instance;

void hook::UpdateTargetHP(uintptr_t rcx, unsigned long hp, uintptr_t r8) {
	// TODO: Notify when hp reaches specific thresholds.
	bns_instance->SetTargetHP(hp);
	if (hp <= 1) {
		bns_instance->SetTargetDead(true);
	}
	return hook::oUpdateTargetHP(rcx, hp, r8);
}

void * hook::UpdateKeybdDevice(uintptr_t rcx, uintptr_t rdx) {
	// TODO: save rcx or continously update rcx..
	//printf("[UpdateKeybdDevice] keybd_device = %p\n", rcx);
	bns_instance->SetKeybdDevice(rcx);
	return hook::oUpdateKeybdDevice(rcx, rdx);
}

void * hook::InventoryEvent(uintptr_t intenvory_slot, unsigned long slot_id, int r8) {
	// TODO: Log and identify looted items..
	return nullptr;
}

void * hook::SendPacket(uintptr_t rcx, uintptr_t rdx, void * data) {
	// Steal rcx and rdx as they should be constant through a login session.
	// Dirty but hey it works :')
	bns_instance->SetSendPacketStructs(rcx, rdx);
	return hook::oSendPacket(rcx, rdx, data);
}

bool * hook::Move(uintptr_t player, float x, float y, float z) {
	printf("[Move] {%ff, %ff, %ff}\n", x, y, z);
	return oMove(player, x, y, z);
}

bool hook::SetupHooks() {
	bns_instance = bns::Bns::getInstance();

	// UpdateKeybdDevice
	Detour_UpdateKeybdDevice->SetupHook((BYTE *)bns_instance->UpdateKeybdDevice,
		(BYTE *)hook::UpdateKeybdDevice);
	Detour_UpdateKeybdDevice->Hook();
	hook::oUpdateKeybdDevice = 
		Detour_UpdateKeybdDevice->GetOriginal<bns::sigs::UpdateKeybdDevice>();

	// UpdateTargetHP
	Detour_UpdateTargetHP->SetupHook((BYTE *)bns_instance->UpdateTargetHP,
		(BYTE *)hook::UpdateTargetHP);
	Detour_UpdateTargetHP->Hook();
	hook::oUpdateTargetHP =
		Detour_UpdateTargetHP->GetOriginal<bns::sigs::UpdateTargetHP>();

	// SendPacket
	Detour_SendPacket->SetupHook((BYTE *)bns_instance->SendPacket,
		(BYTE *)hook::SendPacket);
	Detour_SendPacket->Hook();
	hook::oSendPacket =
		Detour_SendPacket->GetOriginal<bns::sigs::SendPacket>();

	// SendPacket
	Detour_Move->SetupHook((BYTE *)bns_instance->Move,
		(BYTE *)hook::Move);
	Detour_Move->Hook();
	hook::oMove =
		Detour_Move->GetOriginal<bns::sigs::Move>();
	return false;
}

void hook::RemoveHooks() {
	Detour_UpdateKeybdDevice->UnHook();
	Detour_UpdateTargetHP->UnHook();
	Detour_SendPacket->UnHook();
	Detour_Move->UnHook();
}