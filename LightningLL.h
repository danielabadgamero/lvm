#ifndef LIGHTNING_LL
#define LIGHTNING_LL

#include <vector>
#include <string>
#include <any>
#include <functional>

#include "LightningCore.h"

namespace Lightning
{
	namespace LL
	{
		struct Function
		{
			Cell* address{};
			Cell* args[2]{};
		};

		inline std::map<std::string, int> symbols{};
		inline std::map<std::string, Function> functions
		{
			{ "set", {} },			// <a> <b>: set variable <a> to integer <b>.
			{ "print", {} },		// <a> <>: print <a> to the console.
			{ "halt", {} },			// <> <>  : terminates the programme.
		};

		void compile(std::vector<std::string>*, std::vector<std::string>*);
	}
}

#endif