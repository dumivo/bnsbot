#include "sensors.h"
#include "PolyHook.h"

static std::shared_ptr<PLH::Detour> Detour_UpdateTargetHP(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_UpdateKeybdDevice(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_InventoryEvent(new PLH::Detour);

void hook::UpdateTargetHP(uintptr_t rcx, unsigned long hp, uintptr_t r8) {
	// TODO: Notify when hp reaches specific thresholds.
}

void * hook::UpdateKeybdDevice(uintptr_t rcx, uintptr_t rdx, uintptr_t r8) {
	// TODO: save rcx or continously update rcx..
	//printf("[UpdateKeybdDevice] keybd_device = %p\n", rcx);
	return hook::oUpdateKeybdDevice(rcx, rdx, r8);
}

void * hook::InventoryEvent(uintptr_t intenvory_slot, unsigned long slot_id, int r8) {
	// TODO: Log and identify looted items..
	return nullptr;
}

bool hook::SetupHooks() {
	bns::Bns *bns_instance = bns::Bns::getInstance();
	Detour_UpdateKeybdDevice->SetupHook(
		(BYTE *)bns_instance->UpdateKeybdDevice,
		(BYTE *)hook::UpdateKeybdDevice);
	Detour_UpdateKeybdDevice->Hook();
	hook::oUpdateKeybdDevice = 
		Detour_UpdateKeybdDevice->GetOriginal<bns::sigs::UpdateKeybdDevice>();
	return false;
}

void hook::RemoveHooks() {
	Detour_UpdateKeybdDevice->UnHook();
}