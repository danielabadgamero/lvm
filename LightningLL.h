#ifndef LIGHTNING_LL
#define LIGHTNING_LL

#include <vector>
#include <string>
#include <functional>
#include <map>

#include "LightningCore.h"

namespace Lightning
{
	namespace LL
	{
		inline std::vector<std::string> symbols{};
		inline std::map <std::string, std::function<void()>> keywords
		{
			{ "alloc", nullptr },
			{ "print", nullptr },
			{ "println", nullptr },
			{ "set", nullptr },
			{ "add", nullptr },
			{ "mul", nullptr },
			{ "halt", nullptr },
		};

		inline std::string args{};
		inline std::vector<std::string>* bin{};

		void loadFunctions();
		void compile(std::vector<std::string>*, std::vector<std::string>*);
	}
}

#endif