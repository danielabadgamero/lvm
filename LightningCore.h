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
	enum class Mode
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

	struct Symbol
	{
		std::string name{};
		int address{};
	};

	struct Programme
	{
		std::string name{};
		int address{};
	};

	inline std::vector<Programme> loadedProgrammes{};
	inline std::vector<Symbol> symbols{};
	inline std::stack<Cell*> stack{};
	inline Cell* addr{ RAM };
	inline bool running{ true };

	void init();
	void clearScreen();
	void loadProgramme();
}

#endif