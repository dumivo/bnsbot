#include "bns.h"
#include "Memory.h"
#include <mutex>
#include <map>

#define BNS_SHOW_DEBUG_MESSAGES 0

using namespace bns;

bool Bns::has_instance_ = false;
Bns *Bns::instance_ = NULL;

static std::mutex mutex_target_hp;
static std::mutex mutex_target_dead;

Bns *Bns::getInstance() {
	if (!has_instance_) {
		instance_ = new Bns();
		has_instance_ = true;
	}
	return instance_;
}

Bns::Bns() {
	base_client_ = (uintptr_t)GetModuleHandle(NULL);
	base_shipping_ = (uintptr_t)GetModuleHandle(L"bsengine_shipping64.dll");
	base_player_ = GetBasePlayer();

	SendPacket = (sigs::SendPacket)(base_client_ + 0xFB9D60);
	Move = (sigs::Move)(base_shipping_ + 0x1DEE7E0);
	SendAction = (sigs::SendAction)(base_client_ + 0x5313D0); // Mouse and F
	SendKeyboard = (sigs::SendKeyboard)(base_client_ + 0x5322C0);
	ObjectCoord = (sigs::ObjectCoord)(base_shipping_ + 0xA242E0); // Obsolete

	UpdateTargetHP = (sigs::UpdateTargetHP) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x48\x89\x5C\x24\x18\x48\x89\x7C\x24\x20\x41\x54\x41\x55\x41\x56\x48\x83\xEC\x20\x48\x8D\x79\x18\x48\x89\x51\x38",
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	UpdateKeybdDevice = (sigs::UpdateKeybdDevice) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x40\x53\x57\x41\x54\x41\x55\x48\x00\x00\x00\x00\x00\x00\x48\x8B\xD9\x00\x00\x00\x00\x00\x00\x00\x00\x33\xD2",
		"xxxxxxxx??????xxx????????xx");
	InventoryEvent = (sigs::InventoryEvent) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\xFA\x34\x3B\x01\x41\x0F\xB6\xF8\x48\x8B\xF2",
		"xxxxxxxxxxxxxxxxxx????xxxxxxx");
	SendTab = (sigs::SendTab) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\x48\x8B\x0D\xE0\x08\x2E\x01\x48\x85\xC9\x0F\x84\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x48\x85\xD2\x0F\x84\x00\x00\x00\x00\x4C\x89\x64\x24\x28\x00\x00\x00\x00\x00\x00\x00\x4D\x85\xE4\x75\x0D",
		"xxxxxxxxxxxx????xxxxx???????????xxxxx????xxxxx???????xxxxx");
	SendEscape = (sigs::SendEscape) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x41\x54\x48\x83\xEC\x20\x4C\x8B\xE1\x48\x8B\x0D\x90\x03\x2E\x01\x48\x85\xC9\x74\x16\x00\x00\x00\x00\x00\x00\x00\x48\x85\xC9",
		"xxxxxxxxxxxx????xxxxx???????xxx");

	/*
	Send tab
	\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\x48\x8B\x0D\xE0\x08\x2E\x01\x48\x85\xC9\x0F\x84\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x48\x85\xD2\x0F\x84\x00\x00\x00\x00\x4C\x89\x64\x24\x28\x00\x00\x00\x00\x00\x00\x00\x4D\x85\xE4\x75\x0D
	xxxxxxxxxxxx????xxxxx???????????xxxxx????xxxxx???????xxxxx

	Send esc
	\x41\x54\x48\x83\xEC\x20\x4C\x8B\xE1\x48\x8B\x0D\x90\x03\x2E\x01\x48\x85\xC9\x74\x16\x00\x00\x00\x00\x00\x00\x00\x48\x85\xC9
	xxxxxxxxxxxx????xxxxx???????xxx
	*/

	packet_rcx_ = 0;
	packet_rdx_ = 0;

	item.insert(std::pair<char *, char *>("\xA\xC", "Moonstone"));
}

Bns::~Bns() {
	has_instance_ = false;
}

uintptr_t Bns::GetBasePlayer() {
	const BYTE *pattern = (BYTE *)
		"\x48\x8B\x05\x00\x00\x00\x00"  // mov rax, qword ptr ds:[ADR]
		"\x48\x8B\x90\x84\x05\x00\x00"  // mov rdx, qword ptr ds:[rax+584]
		"\x48\x8B\x02"				    // mov rax, qword ptr ds:[rdx]
		"\x48\x8B\x50\x68"			    // mov rdx, qword ptr ds:[rax+68]
		"\x48\x8B\x82\x9C\x02\x00\x00"; // mov rax, qword ptr ds:[29C]
	const char *mask = "xxx????xxxxxxxxxxxxxxxxxxxxx";

	// opcode mov rax, qword ptr ds:[ADR] (we want to find out ADR).
	BYTE *opcode_player = (BYTE *)Pattern(base_shipping_, 0xB000000, pattern, mask);
	if (!opcode_player) {
		printf("ERROR: pOpcode_player\n");
		return -1;
	}

	BYTE base_player_adr_raw[4];
	uintptr_t base_player = 0;

	// Retrieve relative ADR-part
	memcpy(base_player_adr_raw, opcode_player + 0x3, 4 * sizeof(BYTE));

	// Convert byte-array to uintptr_t value.
	for (int i = 0; i < 4; i++) {
		base_player += base_player_adr_raw[i] << 8 * i;
	}

	// Convert relative address to absolute address.
	base_player += (uintptr_t)opcode_player + 0x07;

	return base_player;

}

uintptr_t Bns::GetPlayer() {
	// + 80 = coords
	// + 23c0 = current moving destination
	const std::vector<uintptr_t> offsets = { 0x0, 0x584, 0x0, 0x68, 0x29C, 0x0 };
	return GetAddressByPointer(base_player_, offsets);
}

coord::Coord Bns::GetPlayerCoord() {
	uintptr_t player = Bns::GetPlayer();
	if (!player) {
		printf("Error. player is NULL\n");
		return (coord::Coord{ 0, 0, 0 });
	}
	float *coords = (float *)(player + 0x80);
	coord::Coord coord_struct;
	coord_struct.x = coords[0];
	coord_struct.y = coords[1];
	coord_struct.z = coords[2];
	return coord_struct;
}

bool bns::Bns::PlayerIsBusy() {
	uintptr_t player = Bns::GetPlayer();
	if (!player) {
		return false;
	}
	return *(bool *)(player + 0x23C0 + 0x8);
}

void bns::Bns::SetKeybdDevice(uintptr_t keybd_device) {
	if (keybd_device_ != keybd_device) {
		keybd_device_ = keybd_device;
#if defined (BNS_SHOW_DEBUG_MESSAGES)
		printf("[BNS] Changed keybd_device to %p.\n", (void *)keybd_device_);
#endif
	}
}

uintptr_t bns::Bns::GetKeybdDevice() {
	return keybd_device_;
}

void bns::Bns::SetTargetHP(unsigned long hp) {
	{
		std::lock_guard<std::mutex> lock(mutex_target_hp);
		target_hp_ = hp;
	}
#if defined (BNS_SHOW_DEBUG_MESSAGES)
	printf("[BNS] Target HP = %i.\n", target_hp_);
#endif
}

unsigned long bns::Bns::GetTargetHP() {
	{
		std::lock_guard<std::mutex> lock(mutex_target_hp);
		unsigned long hp = target_hp_;
	}

	return target_hp_;
}

void bns::Bns::SetTargetDead(bool dead) {
	std::lock_guard<std::mutex> lock(mutex_target_dead);
	target_is_dead_ = dead;
}

bool bns::Bns::IsTargetDead() {
	bool dead = false;
	{
		std::lock_guard<std::mutex> lock(mutex_target_dead);
		dead = target_is_dead_;
	}
	return dead;
}

void bns::Bns::SendKeyboardEasy(int a, int b) {
	if (keybd_device_) {
		SendKeyboard(keybd_device_, a, b);
	}
}

void bns::Bns::SendActionEasy(int a, int b) {
	if (keybd_device_) {
		SendAction(keybd_device_, a, b);
	}
}

void bns::Bns::SendTabEasy() {
	if (keybd_device_) {
		SendTab(keybd_device_);
	}
}

void bns::Bns::SendEscEasy() {
	if (keybd_device_) {
		SendEscape(keybd_device_);
	}
}

void bns::Bns::SendPacketEasy(void * data) {
	if (packet_rcx_ && packet_rdx_) {
		SendPacket(packet_rcx_, packet_rdx_, data);
	}
}

void bns::Bns::SetSendPacketStructs(uintptr_t rcx, uintptr_t rdx) {
	packet_rcx_ = rcx;
	packet_rdx_ = rdx;
}
