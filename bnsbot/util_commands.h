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

	class UseEsc : public Command {
	public:
		UseEsc();
		bool Execute();
	};

	// Waits until loading screen is over.
	class WaitLoadingScreen : public Command {
	public:
		WaitLoadingScreen();
		bool Execute();
	};

	// Waits until loading screen is over.
	class SkipCutscene : public Command {
		bool is_robot_ = true;
	public:
		SkipCutscene(bool is_robot);
		SkipCutscene();
		bool Execute();
	};

	// Command for pressing F
	class MouseSpam : public Command {
	public:
		bool Execute();
	};
}