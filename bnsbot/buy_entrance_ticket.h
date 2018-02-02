#pragma once
#include "command.h"

namespace bot {
	class BuyEntranceTicket : public Command {
	public:
		BuyEntranceTicket();
		bool Execute();
	};

	class BuyEntranceTicketF9 : public Command {
	public:
		BuyEntranceTicketF9();
		bool Execute();
	};
}