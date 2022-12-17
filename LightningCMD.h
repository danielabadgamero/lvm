#ifndef LIGHTNING_CMD
#define LIGHTNING_CMD

#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "LightningCore.h"

namespace Lightning
{
	namespace CMD
	{
		inline struct
		{
			std::string cmd{};
			std::map<std::string, std::string> args{};
		} command;

		inline std::map<std::string, std::function<void()>> commandFunctions
		{
			{ std::string{ "exit" }, nullptr },
			{ std::string{ "help" }, nullptr },
			{ std::string{ "mkdir" }, nullptr },
			{ std::string{ "rmdir" }, nullptr },
			{ std::string{ "cd" }, nullptr },
			{ std::string{ "ls" }, nullptr },
			{ std::string{ "touch" }, nullptr },
			{ std::string{ "rm" }, nullptr },
			{ std::string{ "print" }, nullptr },
			{ std::string{ "open" }, nullptr },
		};

		inline std::map<std::string, std::string> commandDescriptions{};

		void loadFunctions();
		bool parseCommand(std::string* input);
		void processCommand();
	}
}

#endif