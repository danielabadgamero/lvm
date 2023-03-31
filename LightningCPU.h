#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <stack>

namespace Lightning::CPU
{
	enum Opcode
	{
		// Data management
		MOV,	//	move source to dAddr
		MGT,	//	MOV if greater flag set
		MLT,	//	MOV if less flag set
		MEQ,	//	MOV if equal flag set
		POP,	//	pop top of stack to dAddr
		RD,		//	read at address in source to dAddr
		WR,		//	write source to address in dAddr
		IN,		//	TBD
		OUT,	//	TBD
		
		// Flow control
		HALT,	//


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