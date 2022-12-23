#ifndef LIGHTNING_LL
#define LIGHTNING_LL

#include <vector>
#include <string>
#include <functional>

#include "LightningCore.h"

namespace Lightning
{
	namespace LL
	{
		inline struct
		{
			std::string name{};
			std::vector<std::string> args{};
		} function;

		inline std::map<std::string, int> symbols{};
		inline std::map<std::string, std::function<void(std::vector<std::string>*)>> functions
		{
			{ "set", {} },			// <a> <b>	: set variable <a> to integer <b>.
			{ "print", {} },		// <a> <>	: print <a> to the console.
			{ "halt", {} },			// <> <>	: terminates the programme.
		};

		void loadFunctions();
		void compile(std::vector<std::string>*, std::vector<std::string>*);
	}
}

#endif