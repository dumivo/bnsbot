#pragma once
#include "command.h"

namespace bot {
	class Loot : public Command {
		
	public:
		Loot();
		bool Execute();
	};
}