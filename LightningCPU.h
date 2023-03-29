#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <stack>

namespace Lightning::CPU
{
	enum Opcode
	{
		//			address == 0				address != 0
		// Data management
		PUSH,	//	Push operand to stack		Push reg to stack
		POP,	//	Pop top of stack			Pop top of stack to reg
		RD,		//	
		WR,		//
		
		// Flow control
		HALT,	//
		JMP,	//


		// Arithmetic and logic
		CMP,	//
		ADD,	//
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