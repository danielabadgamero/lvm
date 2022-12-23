#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <functional>
#include <stack>

namespace Lightning
{
	inline enum class Mode
	{
		CMD,
		TEXT,
		EXEC,
	} mode;

	inline struct Cell
	{
		int value{};
		bool allocated{};
	} RAM[1 << 15];

	inline struct
	{
		int value{};
		bool occupied{};
	} REG[8];

	inline Cell* addr{ RAM };
	inline bool running{ true };

	void init();
	void clearScreen();
	void loadProgramme();
}

#endif