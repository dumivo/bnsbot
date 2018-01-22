#pragma once
#include "command.h"

namespace bot {
	class BuyTicket : public Command {
	public:
		BuyTicket();
		~BuyTicket();
		bool Execute();
	};
}