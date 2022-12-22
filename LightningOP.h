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
			HALT,	// < > < > Finish execution and return to CMD mode, freeing up the resources.
			RMEM,	// <a> <b> Read memory from address <a> into <b>.
			WMEM,	// <a> <b> Write <b> at memory address <a>.
			CALL,	// <a> < > Push instruction's address after <a> to the stack and jump to address <a>.
			RET,	// < > < > Jump to stack's top element and pop it.
			OUT,	// <a> < > Print ASCII value of <a> to the console.
		};

		inline struct
		{
			Opcode opcode{};
			std::vector<Cell*> args;
		} operation;

		inline std::map<Opcode, std::function<Cell*()>> operations
		{
			{ HALT, nullptr },
			{ RMEM, nullptr },
			{ WMEM, nullptr },
			{ CALL, nullptr },
			{ RET, nullptr },
			{ OUT, nullptr },
		};

		void loadOperations();
		bool parseOperation();
		void processOperation();
	}
}

#endif