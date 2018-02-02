#pragma once
namespace bot {
	class Command {
	public:
		Command();
		~Command();
		virtual bool Execute();
	};
}
