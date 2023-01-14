#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

#include "LightningCore.h"

#include <stack>

namespace Lightning::CPU
{
	enum
	{
		HALT,
		SET,
		RMEM,
		WMEM,
		ADD,
		SUB,
		MUL,
		DIV,
		JMP,
		CMP,
		JEQ,
		JNE,
		JGT,
		JLT,
		JGE,
		JLE,
		CALL,
		RET,
		PUSH,
		POP,
	};

	enum
	{
		R0 = Lightning::MAX_RAM,
		R1,
		R2,
		R3,
		R4,
		MAR,
		MDR,
		FLAGS,
	};

	inline int REG[8]{};
	inline std::stack<int> stack{};

	inline int PC{};
	inline int instruction{};

	inline constexpr int FLAGS_EQ{ 0x0001 };
	inline constexpr int FLAGS_NE{ 0x0002 };
	inline constexpr int FLAGS_GT{ 0x0004 };
	inline constexpr int FLAGS_LT{ 0x0008 };

	inline constexpr int FLAGS_GE{ 0x0010 };
	inline constexpr int FLAGS_LE{ 0x0020 };

	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif