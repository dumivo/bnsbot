#pragma once
#include <Windows.h>
#include <stdio.h>
#include <map>

namespace bns {
	namespace sigs {
		typedef void *(__fastcall *SendPacket)(void *rcx, void *rdx, void *r8);
		typedef bool *(__fastcall *Move)(uintptr_t rcx, float xmm1, float xmm2, float xmm3);
		typedef void *(__fastcall *SendAction)(DWORD rcx, DWORD rdx, DWORD r8);
		typedef void *(__fastcall *ObjectCoord)(uintptr_t rcx, uintptr_t rdx);
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
	};
}
