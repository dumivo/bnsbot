#pragma once
#include <Windows.h>
#include <mutex>

#include "bns.h"

namespace bot {
	enum State { Off, Ready, Running, Suspended };
	static std::mutex mutex_bot_state;
	static State bot_state = Off;

	static double cooldown_start_time;
	static HMODULE dll;

	/// <summary> Changes the state of the bot in a thread-safe manner.</summary>
	/// <param name="state"> The new state. </param>
	void ChangeState(State state);
	State GetState();
	void BotMain(LPVOID param);
}