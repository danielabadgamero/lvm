#ifndef LIGHTNING_TEXT
#define LIGHTNING_TEXT

#include <map>
#include <string>
#include <functional>

#include "LightningCore.h"

namespace Lightning
{
	namespace TEXT
	{
		inline struct
		{
			char cmd{};
			int line{};
			std::string content{};
		} command;

		inline std::map<char, std::function<void()>> commandFunctions
		{
			{ '+', nullptr },
			{ '-', nullptr },
			{ '/', nullptr },
		};

		void loadFunctions();
		bool parseCommand(std::string* input);
		void processCommand();
	}
}

#endif