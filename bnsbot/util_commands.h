#pragma once
#include "command.h"
// Some utility (or trivial) commands that are pretty important to have.
namespace bot {
	// Command for waiting (blocking) for a specific time.
	class Wait : public Command {
		unsigned int time_to_wait_ms_;
	public:
		Wait();
		Wait(unsigned int time_to_wait_ms);
		bool Execute();
	};

	// Command for pressing F
	class UseF : public Command {
	public:
		UseF();
		bool Execute();
	};

	class UsePortalF : public Command {
	public:
		UsePortalF();
		bool Execute();
	};

	// Waits until loading screen is over.
	class WaitLoadingScreen : public Command {
	public:
		WaitLoadingScreen();
		bool Execute();
	};

	// Command for pressing F
	class MouseSpam : public Command {
	public:
		bool Execute();
	};

	// Command for pressing F
	class MushinInstancePrologue : public Command {
	public:
		bool Execute();
	};
}