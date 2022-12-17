#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>

typedef std::map<std::string, std::string>* Arguments;

namespace Lightning
{
	enum class Opcode
	{
		HALT,
	};

	enum class Mode
	{
		CMD,
		TEXT,
	};

	inline int RAM[1 << 15];
	inline int REG[8];
	inline Mode mode{ Mode::CMD };

	inline int* addr{ RAM };
	inline bool running{ true };

	void init();
	void clearScreen();
}

#endif