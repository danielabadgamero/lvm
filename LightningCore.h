#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <stack>
#include <bitset>


namespace Lightning
{
	inline enum class Mode
	{
		CMD,
		TEXT,
		EXEC,
	} mode{};

	inline struct Cell
	{
		std::bitset<5> opcode{};
		std::bitset<3> Rd{};
		std::bitset<3> Rs1{};
		std::bitset<3> Rs2{};
		long imm{};
		bool allocated{};
	} RAM[1 << 15]{};

	inline long REG[8];
	inline std::stack<long> stack{};
	inline constexpr int ADDR{ 0 };
	inline constexpr int VAL{ 1 };

	inline Cell* PC{ RAM };
	inline bool running{ true };

	void init();
	void clearScreen();
	void loadProgramme();
}

#endif