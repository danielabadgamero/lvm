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

	struct Programme
	{
		std::string name{};
		int address{};
	};

	inline constexpr Cell* os_func_start{ RAM };
	inline constexpr Cell* prog_start	{ RAM + 1'000 };
	inline constexpr Cell* symb_start	{ RAM + 100'000 };
	inline constexpr Cell* func_start	{ RAM + 1000'000 };

	inline std::vector<Programme> loadedProgrammes{};
	inline std::stack<Cell*> stack{};
	inline Cell* addr{ RAM };
	inline bool running{ true };

	void init();
	void clearScreen();
	void loadProgramme();
}

#endif