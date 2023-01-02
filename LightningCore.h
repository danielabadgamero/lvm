#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <stack>

#include "LightningOP.h"

#undef OUT

namespace Lightning
{
	inline enum class Mode
	{
		CMD,
		TEXT,
		EXEC,
	} mode{};

	inline int REG[8]{};
	inline std::stack<int> stack{};
	inline int RAM[4096]{};

	constexpr int* PC{ REG };
	constexpr int OUT{ 1 };
	constexpr int IN1{ 2 };
	constexpr int IN2{ 3 };
	constexpr int ADDR1{ 4 };
	constexpr int ADDR2{ 5 };
	constexpr int TEST{ 6 };
	constexpr int COUNTER{ 7 };

	constexpr int INPUT{ 512 };

	inline bool running{ true };

	void init();
	void clearScreen();
	void writeBin(Lightning::OP::Opcode opcode, unsigned char Rd, unsigned char Rs1, unsigned char Rs2, short imm = 0);
}

#endif