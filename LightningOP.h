#ifndef LIGHTNING_OP
#define LIGHTNING_OP

#undef OUT

#include <functional>
#include <vector>
#include <map>
#include <string>

#include "LightningCore.h"

namespace Lightning
{
	namespace OP
	{
		enum Opcode
		{
			HALT,	// < > < >		Finish execution and return to CMD mode, freeing up the resources.
			RWMEM,	// <a> <b>		Read memory from address <a> and write it <b> cells later.
			RMEM,	// <a> <b>		Read memory from address <a> into <b>.
			WMEM,	// <a> <b>		Write <b> at memory address <a>.
			OUT,	// <a> < >		Print ASCII value of <a> to the console.
		};

		inline struct
		{
			Opcode opcode{};
			std::vector<Cell*> args;
		} operation;

		inline std::map<Opcode, std::function<Cell*()>> operations
		{
			{ HALT, nullptr },
			{ RWMEM, nullptr },
			{ RMEM, nullptr },
			{ WMEM, nullptr },
			{ OUT, nullptr },
		};

		void loadOperations();
		bool parseOperation();
		void processOperation();
	}
}

#endif