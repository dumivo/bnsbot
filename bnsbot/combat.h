#pragma once
#include <Windows.h>
#include "command.h"
// "keybd device structure" @ Mouse2: (rcx-0x18)+0x8dd88
namespace bot {
	// Command for killing a target. Heh.
	class Combat : public Command {
	public:
		Combat();
		~Combat();
		bool Execute();
	};

	class CombatSpin : public Combat {
	public:
		CombatSpin();
		~CombatSpin();
		bool Execute();
	};

}