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
		enum Func
		{
			cout,	// args: <std::string> ret: <void>

			total_funcs
		};

		struct Symbol
		{
			std::string name{};
			int address{};
		};

		inline struct
		{
			Cell* address{};
			std::vector<Cell*> args{};
			Cell* ret{};
		} procedures[total_funcs];

		inline std::vector<Symbol> symbols{};

		void loadFunctions();
		void compile(std::vector<std::string>*);
	}
}

#endif