#ifndef LIGHTNING_CMD
#define LIGHTNING_CMD

#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <functional>

namespace Lightning
{
	namespace CMD
	{
		inline struct
		{
			std::string cmd{};
			std::map<std::string, std::string> args{};
		} command{};

		inline std::map<std::string, std::function<void()>> commandFunctions
		{
			{ "exit", nullptr },
			{ "help", nullptr },
			{ "mkdir", nullptr },
			{ "rmdir", nullptr },
			{ "cd", nullptr },
			{ "ls", nullptr },
			{ "touch", nullptr },
			{ "rm", nullptr },
			{ "print", nullptr },
			{ "open", nullptr },
			{ "clear", nullptr },
		};

		inline std::map<std::string, std::string> commandDescriptions{};

		void loadFunctions();
		bool parseCommand(std::string*);
		void processCommand();
	}
}

#endif