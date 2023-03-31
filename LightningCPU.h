#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <stack>

namespace Lightning::CPU
{
	enum Opcode
	{
		HALT,	//	set selected chip to dAddr

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

		// Arithmetic and logic
		ADD,	//	add source to dAddr
		MUL,	//	multiply dAddr times source
		DIV,	//	divide dAddr by source
		CMP,	//	compare dAddr with source and set flags
		AND,	//	set dAddr to its bitwise and with source
		NOT,	//	set dAddr to its bitwise negation
	};

	inline struct Instruction
	{
		unsigned char aMode : 1;	//	defines interpretation of immediate (if existent)
		unsigned char dAddr : 3;	//	destination address
		unsigned char opcode : 4;	//	16 different opcodes
		
		Instruction& operator=(unsigned char data)
		{
			opcode = (data & 0b1111'0000) >> 4;
			dAddr = (data & 0b0000'1110) >> 1;
			aMode = (data & 0b0000'0001);

			return *this;
		}
	} instruction{};

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

	inline std::stack<int> stack{};

	void decode();
	int cycle(void*);
}

#endif