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
	base_shipping_ = (uintptr_t)LoadLibrary(L"bsengine_shipping64.dll");
	base_player_ = GetBasePlayer();
	base_target_hp_ = GetBaseTargetHP();
	base_keybd_device_ = GetBaseKeybdDevice();

	cooldown_start_time_ = 0;

	SendPacket = (sigs::SendPacket)(base_client_ + 0xFB9D60);
	Move = (sigs::Move)(base_shipping_ + 0x1DEE7E0);
	SendAction = (sigs::SendAction)(base_client_ + 0x5313D0); // Mouse and F
	SendKeyboard = (sigs::SendKeyboard)(base_client_ + 0x5322C0);
	ObjectCoord = (sigs::ObjectCoord)(base_shipping_ + 0xA242E0); // Obsolete

	Exc = (sigs::Exc)(base_shipping_ + 0x828B10);

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
	SendMove2 = (sigs::SendMove2) Pattern(base_shipping_, 0xB000000,
		(BYTE *)"\x40\x53\x48\x83\xEC\x60\x48\x8B\x05\xCB\x44\x13\x03\x0F\x29\x74\x24\x50\x0F\x29\x7C\x24\x40\x48\x85\xC0\x44\x0F\x29\x44\x24\x30\x44\x0F\x29\x4C\x24\x20\x0F\x28\xFB\x44\x0F\x28\xC2\x44\x0F\x28\xC9",
		"xxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	ExitLoadingScreen = (sigs::ExitLoadingScreen) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x40\x53\x48\x83\xEC\x40\x48\x83\x79\x18\x00\x48\x8B\xD9\x0F\x84\x00\x00\x00\x00\x48\x83\xC9\xFF",
		"xxxxxxxxxxxxxxxx????xxxx");
	SendKey = (sigs::SendKey) Pattern(base_client_, 0xB000000,
		(BYTE *)"\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x00\x00\x00\x00\x00\x00\x48\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x45\x0F\xB6\xF0\x48\x8B\xFA\x48\x8B\xD9\x4C\x8B\x79\x08\x4D\x85\xFF",
		"xxxxxxxxxxxxx??????x????????????????xxxxxxxxxxxxxxxxx");
	SendKeyUp = (sigs::SendKeyUp) Pattern(base_client_, 0x1000000,
		(BYTE *)"\x40\x57\x41\x54\x41\x55\x41\x56\x41\x57\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x2B\xE0\x48\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x4C\x8B\xEA\x48\x8B\xD9\x48\x8B\x05\x6B\xF4\x9A\x00",
		"xxxxxxxxxxx????x????xxxx????????????????????????????????xxxxxxxxxxxxx");
	EInterfaceGetInstance = (sigs::EInterfaceGetInstance) GetProcAddress((HMODULE)base_shipping_, "EInterfaceGetInstance");

	cutscene_if_exists_ = Pattern(base_client_, 0x1000000,
		(BYTE *)"\x0F\x85\x00\x00\x00\x00\x33\xC0\x48\x83\xC9\xFF\x48\x8D\x7B\x20\x66\xF2\xAF\x48\xF7\xD1\x48\x83\xF9\x01",
		"xx????xxxxxxxxxxxxxxxxxxxx");

	/*

	\x40\x57\x41\x54\x41\x55\x41\x56\x41\x57\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x2B\xE0\x48\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x4C\x8B\xEA\x48\x8B\xD9\x48\x8B\x05\x6B\xF4\x9A\x00
	xxxxxxxxxxx????x????xxxx????????????????????????????????xxxxxxxxxxxxx

	base monsterhp
	\x48\x8B\x0D\x2C\x07\x1E\x01\x48\x89\x74\x24\x30\x33\xF6\x48\x85\xC9
	xxx????xxxxxxxxxx
	SendMove2
	\x40\x53\x48\x83\xEC\x60\x48\x8B\x05\xCB\x44\x13\x03\x0F\x29\x74\x24\x50\x0F\x29\x7C\x24\x40\x48\x85\xC0\x44\x0F\x29\x44\x24\x30\x44\x0F\x29\x4C\x24\x20\x0F\x28\xFB\x44\x0F\x28\xC2\x44\x0F\x28\xC9
	xxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	ExitLoadingScreen
	\x40\x53\x48\x83\xEC\x40\x48\x83\x79\x18\x00\x48\x8B\xD9\x0F\x84\x00\x00\x00\x00\x48\x83\xC9\xFF
	xxxxxxxxxxxxxxxx????xxxx

	{
	0xB0, 0x98, 0x39, 0x41, 0x01, 0x00, 0x00, 0x00, 0x10, 0xE0, 0x5C, 0x0A, 0x01, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x49, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x57, 0x00, 0x30, 0x00, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	sendkey
	\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x00\x00\x00\x00\x00\x00\x48\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x45\x0F\xB6\xF0\x48\x8B\xFA\x48\x8B\xD9\x4C\x8B\x79\x08\x4D\x85\xFF
	xxxxxxxxxxxxx??????x????????????????xxxxxxxxxxxxxxxxx

	Pattern to jnz instruction of PushCutscene. patch to jmp and you'll skip every cutscene.
	\x0F\x85\x00\x00\x00\x00\x33\xC0\x48\x83\xC9\xFF\x48\x8D\x7B\x20\x66\xF2\xAF\x48\xF7\xD1\x48\x83\xF9\x01
	xx????xxxxxxxxxxxxxxxxxxxx
	0f 85 45 08 00 00
	e9 46 08 00 00 90

	*/

	packet_rcx_ = 0;
	packet_rdx_ = 0;

	item.insert(std::pair<char *, char *>("\xA\xC", "Moonstone"));
}

uintptr_t ScanBaseRelative(uintptr_t start_address, const BYTE *pattern, const char *mask,
	unsigned int size, unsigned int offset, unsigned int len) {
	BYTE *opcode = (BYTE *)Pattern(start_address, 0xB000000, pattern, mask);
	if (!opcode) {
		printf("ERROR: pOpcode_player\n");
		return 0;
	}

	BYTE *base_adr_raw = new BYTE[len];
	uintptr_t base_adr = 0;

	// Retrieve relative ADR-part
	memcpy(base_adr_raw, opcode + offset, len * sizeof(BYTE));

	// Convert byte-array to uintptr_t value.
	for (unsigned int i = 0; i < len; i++) {
		base_adr += base_adr_raw[i] << 8 * i;
	}

	delete base_adr_raw;

	// Convert relative address to absolute address.
	base_adr += (uintptr_t)opcode + size;

	return base_adr;
}

uintptr_t Bns::GetBasePlayer() {
	const BYTE *pattern = (BYTE *)
		"\x48\x8B\x05\x00\x00\x00\x00"  // mov rax, qword ptr ds:[ADR]
		"\x48\x8B\x90\x84\x05\x00\x00"  // mov rdx, qword ptr ds:[rax+584]
		"\x48\x8B\x02"				    // mov rax, qword ptr ds:[rdx]
		"\x48\x8B\x50\x68"			    // mov rdx, qword ptr ds:[rax+68]
		"\x48\x8B\x82\x9C\x02\x00\x00"; // mov rax, qword ptr ds:[29C]
	const char *mask = "xxx????xxxxxxxxxxxxxxxxxxxxx";

	/*
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
	base_player += (uintptr_t)opcode_player + 0x07;*/

	return ScanBaseRelative(base_shipping_, pattern, mask, 0x7, 0x3, 0x4);

}

uintptr_t Bns::GetPlayerAddress() {
	// + 80 = coords
	// + 23c0 = current moving destination
	const std::vector<uintptr_t> offsets = { 0x0, 0x584, 0x0, 0x68, 0x29C, 0x0 };
	return GetAddressByPointer(base_player_, offsets);
}

coord::Coord Bns::GetPlayerCoord() {
	uintptr_t player = Bns::GetPlayerAddress();
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

void bns::Bns::SetSleep(bool mode) {
	is_sleeping_ = mode;
}

bool bns::Bns::IsSleeping() {
	return is_sleeping_;
}

uintptr_t Bns::GetBaseTargetHP() {
	const BYTE *pattern = (BYTE *) "\x48\x8B\x0D\x2C\x07\x1E\x01\x48\x89\x74\x24\x30\x33\xF6\x48\x85\xC9";
	const char *mask = "xxx????xxxxxxxxxx";
	return ScanBaseRelative(base_client_, pattern, mask, 0x7, 0x3, 0x4);
}

uintptr_t bns::Bns::GetTargetHPAddress() {
	const std::vector<uintptr_t> offsets = { 0x0, 0x88, 0x38, 0x788 };
	return GetAddressByPointer(base_target_hp_, offsets);
}

uintptr_t Bns::GetBaseKeybdDevice() {
	const BYTE *pattern = (BYTE *) "\x48\x8B\x0D\x08\xB0\x52\x01\x48\x00\x00\x00\x00\x00\x00\x00\x74\x1B\x00\x00\x00\x00\x00\x00\x00\x48\x83\xC1\x08\x48\x8B\x01\x48\x8B\xD3\xFF\x50\x08";
	const char *mask = "xxx????x???????xx???????xxxxxxxxxxxxx";
	return ScanBaseRelative(base_client_, pattern, mask, 0x7, 0x3, 0x4);
}

uintptr_t bns::Bns::GetKeybdDevice() {
	return keybd_device_;

}


void bns::Bns::RefreshKeybdDevice() {
	// Split them up because of an add rcx, 0x8
	const std::vector<uintptr_t> offsets1 = { 0x0, 0x88, 0x0 };
	const std::vector<uintptr_t> offsets2 = { 0x18, 0x0 };
	uintptr_t adr1 = GetAddressByPointer(base_keybd_device_, offsets1);
	if (!adr1) {
		printf("Error refreshkeybddvice\n");
		return;
	}

	adr1 = GetAddressByPointer(adr1, offsets2);
	if (!adr1) {
		printf("Error refreshkeybddvice\n");
		return;
	}
	adr1 += 0x128E8;
	adr1 -= 0x18;
	//adr1 += 0x8DD90;
	printf("Updates keybd_device to %p\n", (void *)adr1);
	keybd_device_ = adr1;
}

bool bns::Bns::PlayerIsBusy() {
	uintptr_t player = Bns::GetPlayerAddress();
	if (!player) {
		return false;
	}
	return *(bool *)(player + 0x23C0 + 0x8);
}

void bns::Bns::SetTargetHP(unsigned long hp) {
	{
		std::lock_guard<std::mutex> lock(mutex_target_hp);
		target_hp_ = hp;
	}
#if defined (BNS_SHOW_DEBUG_MESSAGES)
	//printf("[BNS] Target HP = %i.\n", target_hp_);
#endif
}

unsigned long bns::Bns::GetTargetHP() {
	/*{
		std::lock_guard<std::mutex> lock(mutex_target_hp);
		unsigned long hp = target_hp_;
	}

	return target_hp_;*/
	uintptr_t hp_adr = GetTargetHPAddress();
	if (hp_adr) {
		return *(unsigned long *)hp_adr;
	}
	else {
		return 0;
	}
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
	uintptr_t keybd_device = GetKeybdDevice();
	if (keybd_device) {
		SendKeyboard(keybd_device, a, b);
	}
}

void bns::Bns::SendActionEasy(int a, int b) {
	uintptr_t keybd_device = GetKeybdDevice();
	if (keybd_device) {
		uintptr_t res = (uintptr_t)SendAction(keybd_device, a, b);
		if (!(res & 1)) {
			//printf("Sendaction returned 0 (%p)\n", (void *)res);
		}
	}
}

void bns::Bns::SendTabEasy() {
	uintptr_t keybd_device = GetKeybdDevice();
	if (keybd_device) {
		SendTab(keybd_device);
	}
}

void bns::Bns::SendEscEasy() {
	if (keybd_device_) {
		SendEscape(keybd_device_);
	}
}

void bns::Bns::SendKeyEasy(unsigned char id) {
	unsigned char data[0x20] =
	{
		id  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	uintptr_t keybd_device = GetKeybdDevice();
	if (keybd_device) {
		SendKey(keybd_device, data);
	}
	
}

void bns::Bns::SendKeyUpEasy(unsigned char id) {
	unsigned char data[0x20] =
	{
		id  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
	uintptr_t keybd_device = GetKeybdDevice();
	if (keybd_device) {
		SendKeyUp(keybd_device, data);
	}
}

void bns::Bns::SendKeyEasyOnce(unsigned char id) {
	SendKeyEasy(id);
	Sleep(25);
	SendKeyUpEasy(id);
	Sleep(50);
}

void bns::Bns::SendPacketEasy(void * data) {
	uintptr_t packet_rcx = GetAddressByPointer(base_client_, std::vector<uintptr_t> {0x01816148, 0x0});
	uintptr_t packet_rdx = GetAddressByPointer(base_client_, std::vector<uintptr_t> {0x018140E0, 0x48, 0x0});
	if (packet_rcx && packet_rdx) {
		printf("Sending packet with: %p %p\n", (void *)packet_rcx, (void *)packet_rdx);
		SendPacket(packet_rcx, packet_rdx, data);
	}
	else {
		printf("[SendPacketEasy] Error\n");
	}
	
}

bool bns::Bns::SendMoveEasy(const coord::Coord & destination) {
	uintptr_t bns_interface = EInterfaceGetInstance();
	if (bns_interface) {
		return SendMove2(bns_interface, destination.x, destination.y, destination.z);
	}
	printf("[SendMoveEasy] Error - interface is NULL\n");
	return false;
}

void bns::Bns::SetSendPacketStructs(uintptr_t rcx, uintptr_t rdx) {
	packet_rcx_ = rcx;
	packet_rdx_ = rdx;
}

bool bns::Bns::SkipCutscene(bool skip) {
	if (!cutscene_if_exists_) {
		return false;
	}
	printf("Cutscene_if_exists = %p\n", (void *) cutscene_if_exists_);
	// 0f 85 45 08 00 00
	// e9 46 08 00 00 90
	unsigned char *patch = (unsigned char *)cutscene_if_exists_;
	patch[0] = 0xE9; // JMP
	memcpy(patch + 0x1, patch + 0x2, 0x4); // Address
	// JMP opcode is one byte less than JNE, that's why we have to adjust address by one.
	patch[1] += 0x1;
	patch[5] = 0x90;

	return true;
}

double bns::Bns::GetCooldownStartTime() {
	return cooldown_start_time_;
}

void bns::Bns::SetCooldownStartTime() {
	cooldown_start_time_ = clock();
}
