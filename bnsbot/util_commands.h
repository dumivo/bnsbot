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
		~Wait();
		bool Execute();
	};

	// Command for pressing F
	class UseF : public Command {
	public:
		UseF();
		~UseF();
		bool Execute();
	};

	class UsePortalF : public Command {
	public:
		UsePortalF();
		~UsePortalF();
		bool Execute();
	};

	class UseEsc : public Command {
	public:
		UseEsc();
		~UseEsc();
		bool Execute();
	};

	// Waits until loading screen is over.
	class WaitLoadingScreen : public Command {
	public:
		WaitLoadingScreen();
		~WaitLoadingScreen();
		bool Execute();
	};

	// Waits until loading screen is over.
	class SkipCutscene : public Command {
		bool is_robot_ = true;
	public:
		SkipCutscene(bool is_robot);
		SkipCutscene();
		~SkipCutscene();
		bool Execute();
	};
}