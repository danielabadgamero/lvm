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
		inline std::string line{};
		inline std::vector<std::string>* bin{};
		inline std::map<std::string, std::function<void()>> keywords
		{
			{ "func", nullptr },
			{ "alloc", nullptr },
		};
		
		void loadFunctions();
		void compile(std::vector<std::string>*, std::vector<std::string>*);
	}
}

#endif