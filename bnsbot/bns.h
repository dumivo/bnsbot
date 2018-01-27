#pragma once
#include <Windows.h>
#include <stdio.h>
#include <map>
#include "coord.h"

namespace bns {
	namespace sigs {
		typedef void *(__fastcall *SendPacket)(uintptr_t rcx, uintptr_t rdx, void *data);
		typedef bool *(__fastcall *Move)(uintptr_t player, float x, float y, float z);
		typedef void *(__fastcall *SendAction)(uintptr_t rcx, int rdx, int r8);
		typedef void *(__fastcall *SendKeyboard)(uintptr_t rcx, int rdx, int r8);
		typedef void *(__fastcall *SendTab)(uintptr_t rcx);
		typedef void *(__fastcall *SendEscape)(uintptr_t rcx);
		typedef void *(__fastcall *ObjectCoord)(uintptr_t rcx, uintptr_t rdx);
		typedef void  (__fastcall *UpdateTargetHP)(uintptr_t rcx, unsigned long hp, uintptr_t r8);
		typedef void *(__fastcall *UpdateKeybdDevice)(uintptr_t rcx, uintptr_t rdx);
		typedef void *(__fastcall *InventoryEvent)(uintptr_t intenvory_slot, unsigned long slot_id, int r8);
		typedef bool (__fastcall *Exc)(uintptr_t rcx);
		typedef bool (__fastcall *SendMove2)(uintptr_t bns_interface, float x, float y, float z);
		typedef uintptr_t(__fastcall *EInterfaceGetInstance)();
		typedef void *(__fastcall *ExitLoadingScreen)(uintptr_t rcx);
		typedef void *(__fastcall *SendKey)(uintptr_t rcx, char *data, int r8);
	}

	class Bns {
	private:
		// Base addresses of modules.
		uintptr_t base_client_, base_shipping_;
		// Base addresses derived from base addresses of modules.
		uintptr_t base_player_, base_target_hp_, base_keybd_device_;
		// Base structures, devices, things you have to pass as arguments..
		uintptr_t keybd_device_;

		unsigned long target_hp_;
		bool target_is_dead_;

		// Stolen structs for SendPacket
		uintptr_t packet_rcx_, packet_rdx_;

		bool is_sleeping_ = false;

		static bool has_instance_;
		static Bns *instance_;
		Bns();

		uintptr_t GetBasePlayer();
		uintptr_t GetBaseTargetHP();
		uintptr_t GetBaseKeybdDevice();
		
	public:
		std::map<char *, char *> item;

		static Bns *getInstance();
		~Bns();

		// Returns current Player object.
		uintptr_t GetPlayerAddress();
		uintptr_t GetTargetHPAddress();
		// Returns the coordinates of the player.
		// Returns the null-vector if player is not valid (eg in loading screen).
		coord::Coord GetPlayerCoord();

		// Sets to sleep mode, disables some functionality
		void SetSleep(bool);
		// Returns sleep mode
		bool IsSleeping();

		// Returns true, if the player is busy moving to a destination
		// false, if otherwise.
		bool PlayerIsBusy();

		void SetKeybdDevice(uintptr_t keybd_device);
		uintptr_t GetKeybdDevice();

		void SetTargetHP(unsigned long hp);
		unsigned long GetTargetHP();

		void SetTargetDead(bool dead);
		bool IsTargetDead();

		void SendKeyboardEasy(int a, int b);
		void SendActionEasy(int a, int b);
		void SendTabEasy();
		void SendEscEasy();
		void SendPacketEasy(void *data);
		bool SendMoveEasy(const coord::Coord &destination);

		void SetSendPacketStructs(uintptr_t rcx, uintptr_t rdx);

		// Bns executeable functions
		sigs::Move Move;
		sigs::SendPacket SendPacket;
		sigs::SendAction SendAction;
		sigs::ObjectCoord ObjectCoord;
		sigs::SendKeyboard SendKeyboard;
		sigs::SendTab SendTab;
		sigs::SendEscape SendEscape;
		sigs::UpdateTargetHP UpdateTargetHP;
		sigs::UpdateKeybdDevice UpdateKeybdDevice;
		sigs::InventoryEvent InventoryEvent;
		sigs::Exc Exc;
		sigs::EInterfaceGetInstance EInterfaceGetInstance;
		sigs::SendMove2 SendMove2;
		sigs::ExitLoadingScreen ExitLoadingScreen;
		sigs::SendKey SendKey;
	};
}