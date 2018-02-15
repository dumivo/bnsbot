#include "sensors.h"
#include "PolyHook.h"

static std::shared_ptr<PLH::Detour> Detour_UpdateTargetHP(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_UpdateKeybdDevice(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_InventoryEvent(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_SendPacket(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_Move(new PLH::Detour);
static std::shared_ptr<PLH::Detour> Detour_Exc(new PLH::Detour);

static bns::Bns *bns_instance;


void * hook::InventoryEvent(uintptr_t intenvory_slot, unsigned long slot_id, int r8) {
	// TODO: Log and identify looted items..
	return nullptr;
}

static uintptr_t pck_rcx_old = 0, pck_rdx_old = 0;
void * hook::SendPacket(uintptr_t rcx, uintptr_t rdx, void * data) {
	// Steal rcx and rdx as they should be constant through a login session.
	// Dirty but hey it works :')
	/*if (data) {
		int k = 0;
		printf("const char data[]=\n");s
		printf("{\n");
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 16; j++) {
				printf("0x%02X, ", ((unsigned char *)data)[j + k]);
			}
			k += 16;
			printf("\n");
		}
		printf("};\n");
	}*/
	if (rcx != pck_rcx_old || rdx != pck_rdx_old) {
		printf("[SendPacket] Snitched some juicy structs %p %p\n", (void *)rcx, (void*)rdx);
		bns_instance->SetSendPacketStructs(rcx, rdx);
		pck_rcx_old = rcx;
		pck_rdx_old = rdx;
	}
	
	return hook::oSendPacket(rcx, rdx, data);
}

bool hook::Move(uintptr_t player, float x, float y, float z) {
	//bool *rtn = oMove(player, x, y, z);
	printf("{ %ff, %ff, %ff },\n", x, y, z);
	return true;
	//return oMove(player, x, y, z);
}


bool hook::SetupHooks() {
	bns_instance = bns::Bns::getInstance();

	// UpdateKeybdDevice
	/*Detour_UpdateKeybdDevice->SetupHook((BYTE *)bns_instance->UpdateKeybdDevice,
		(BYTE *)hook::UpdateKeybdDevice);
	Detour_UpdateKeybdDevice->Hook();
	hook::oUpdateKeybdDevice = 
		Detour_UpdateKeybdDevice->GetOriginal<bns::sigs::UpdateKeybdDevice>();

	// UpdateTargetHP
	Detour_UpdateTargetHP->SetupHook((BYTE *)bns_instance->UpdateTargetHP,
		(BYTE *)hook::UpdateTargetHP);
	//Detour_UpdateTargetHP->Hook();
	//hook::oUpdateTargetHP =
	//	Detour_UpdateTargetHP->GetOriginal<bns::sigs::UpdateTargetHP>();

	// SendPacket
	Detour_SendPacket->SetupHook((BYTE *)bns_instance->SendPacket,
		(BYTE *)hook::SendPacket);
	Detour_SendPacket->Hook();
	hook::oSendPacket =
		Detour_SendPacket->GetOriginal<bns::sigs::SendPacket>();*/

	// Move
	/*Detour_Move->SetupHook((BYTE *)bns_instance->Move,
		(BYTE *)hook::Move);
	Detour_Move->Hook();
	hook::oMove =
		Detour_Move->GetOriginal<bns::sigs::Move>();*/

	// Exc
	//Detour_Exc->SetupHook((BYTE *)bns_instance->Exc,
		//(BYTE *)hook::Exc);
	//Detour_Exc->Hook();
	//hook::oExc =
	//	Detour_Exc->GetOriginal<bns::sigs::Exc>();*/
	return false;
}

void hook::RemoveHooks() {
	//Detour_UpdateKeybdDevice->UnHook();
	//Detour_UpdateTargetHP->UnHook();
	//Detour_SendPacket->UnHook();
	//Detour_Move->UnHook();
	//Detour_Exc->UnHook();
}