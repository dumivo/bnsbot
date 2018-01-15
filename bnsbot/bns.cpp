#include "bns.h"
#include "Memory.h"
#include <map>

using namespace bns;

bool Bns::has_instance_ = false;
Bns *Bns::instance_ = NULL;


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

	SendPacket = (sigs::SendPacket)(base_client_ + 0xF911E0);
	Move = (sigs::Move)(base_shipping_ + 0x1DED1D0);
	SendAction = (sigs::SendAction)(base_client_ + 0x5444D0);
	ObjectCoord = (sigs::ObjectCoord)(base_shipping_ + 0xA242E0);

	item.insert(std::pair<char *, char *>("\xA\xC", "Moonstone"));
}

Bns::~Bns() {
	has_instance_ = false;
}

uintptr_t Bns::GetBasePlayer() {
	BYTE *pattern = (BYTE *)
		"\x48\x8B\x05\x7B\xC2\x0B\x03"  // mov rax, qword ptr ds:[ADR]
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