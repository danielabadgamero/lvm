#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>

namespace Lightning
{
	int RAM[1 << 15];
	int REG[8];

	int* addr{ RAM };
	bool running{ true };

	enum class OP
	{
		HALT,
	};

	void handleCommand(std::string* command)
	{
		int argSep{ static_cast<int>(command->find(' ')) };
		if (argSep)
	}
}

#endif