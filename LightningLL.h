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
			{ "cout_int", {} },		// <a> <b>: print <b> values starting at <a> as integers.
			{ "cout_char", {} },	// <a> <b>: print <b> values starting at <b> as chars.
			{ "halt", {} },			// <> <>  : terminates the programme.
		};

		void loadFunctions();
		void compile(std::vector<std::string>*);
	}
}

#endif