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
		inline std::map<std::string, int> symbols{};

		void loadFunctions();
		void compile(std::vector<std::string>*, std::vector<std::string>*);
	}
}

#endif