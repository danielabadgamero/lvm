#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <stack>

namespace Lightning::CPU
{
	enum Opcode
	{

	};

	inline union
	{
		struct
		{
			unsigned short operand : 24;	// 24-bit immediate
			unsigned short address : 3;		// Address specifier
			unsigned short opcode : 5;		// 32 different opcodes
		} bitfields;

		unsigned short instruction{};
	} ir{};

	enum Reg
	{
		ax = 1,
		bx,
		cx,
		dx,
		pc,
		pb,
		tr,
	};

	inline int regs[7]{};

	inline std::stack<short> stack{};

	void decode();
	int cycle(void*);
}

#endif