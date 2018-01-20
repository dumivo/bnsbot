#pragma once
#include <Windows.h>
#include <stdio.h>
#include <map>

namespace bns {
	namespace sigs {
		typedef void *(__fastcall *SendPacket)(void *rcx, void *rdx, void *data);
		typedef bool *(__fastcall *Move)(uintptr_t player, float x, float y, float z);
		typedef void *(__fastcall *SendAction)(uintptr_t rcx, int rdx, int r8);
		typedef void *(__fastcall *SendKeyboard)(uintptr_t rcx, int rdx, int r8);
		typedef void *(__fastcall *ObjectCoord)(uintptr_t rcx, uintptr_t rdx);
		typedef void  (__fastcall *UpdateTargetHP)(uintptr_t rcx, unsigned long hp, uintptr_t r8);
		typedef void *(__fastcall *UpdateKeybdDevice)(uintptr_t rcx, uintptr_t rdx, uintptr_t r8);
		typedef void *(__fastcall *InventoryEvent)(uintptr_t intenvory_slot, unsigned long slot_id, int r8);

	}

	class Bns {
	private:
		// Base addresses of modules.
		uintptr_t base_client_, base_shipping_;

		// Base addresses derived from base addresses of modules.
		uintptr_t base_player_;

		static bool has_instance_;
		static Bns *instance_;
		Bns();

		uintptr_t GetBasePlayer();
	public:
		std::map<char *, char *> item;

		static Bns *getInstance();
		~Bns();

		// Returns current Player object.
		uintptr_t GetPlayer();

		// Bns executeable functions
		sigs::Move Move;
		sigs::SendPacket SendPacket;
		sigs::SendAction SendAction;
		sigs::ObjectCoord ObjectCoord;
		sigs::SendKeyboard SendKeyboard;
		sigs::UpdateTargetHP UpdateTargetHP;
		sigs::UpdateKeybdDevice UpdateKeybdDevice;
		sigs::InventoryEvent InventoryEvent;
	};
}