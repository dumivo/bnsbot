#pragma once
#include "command.h"

namespace bot {
	class BuyEntranceTicket : public Command {
	public:
		BuyEntranceTicket();
		~BuyEntranceTicket();
		bool Execute();
	};
}