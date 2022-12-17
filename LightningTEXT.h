#ifndef LIGHTNING_TEXT
#define LIGHTNING_TEXT

#include <map>
#include <string>
#include <functional>

namespace Lightning
{
	namespace TEXT
	{
		inline struct
		{
			char cmd{};
			int line{};
			std::string content{};
		} command{};

		inline std::map<char, std::function<void()>> commandFunctions
		{
			{ '+', nullptr },
			{ '-', nullptr },
			{ '/', nullptr },
			{ '?', nullptr },
		};

		inline std::map<char, std::string> commandDescriptions{};

		void loadFunctions();
		bool parseCommand(std::string*);
		void processCommand();
	}
}

#endif