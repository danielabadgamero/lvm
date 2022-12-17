#ifndef LIGHTNING_CMD
#define LIGHTNING_CMD

#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "LightningCore.h"

typedef std::map<std::string, std::string>* Arguments;

namespace Lightning
{
	namespace CMD
	{
		inline std::map<std::string, std::function<void(Arguments)>> commandFunctions
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

		void loadFunctions();
	}
}

#endif