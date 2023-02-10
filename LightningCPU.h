#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

#include <stack>

namespace Lightning::CPU
{
	inline SDL_Thread* thread;

	/*
	* Instruction format 1:
	* 
	* |     4 bits     |   3 bits   | 1b |           8 bits               |
	* +----------------+------------+----+--------------------------------+
	* |                |            | 0  |                                |
	* |     opcode     |   rDest    | or |  rSource (0) or immediate (1)  |
	* |                |            | 1  |                                |
	* +----------------+------------+----+--------------------------------+
	* 
	* Instruction format 2:
	* 
	* |     4 bits     |   3 bits   | 1b |     5 bits         |   3 bits   |
	* +----------------+------------+----+--------------------+------------+
	* |                |            | 0  |    rSource (0)     |            |
	* |     opcode     |   rDest    | or |        or          |  condFlag  |
	* |                |            | 1  |   immediate (1)    |            |
	* +----------------+------------+----+--------------------+------------+
	* 
	*/

	enum Opcode
	{
		HLT,	// halt.
		MOV,	// <rd> <0/1> <rs/imm>: Store value in register rs (0) or immediate (1) in register rd.
		ADD,	// <rd> <0/1> <rs/imm>: Store addition of register rs (0) or immediate (1) and register rd.
		SUB,	// <rd> <0/1> <rs/imm>: Store substraction of register rs (0) or immediate (1) and register rd.
		MUL,	// <rd> <0/1> <rs/imm>: Store product of register rs (0) or immediate (1) and register rd.
		DIV,	// <rd> <0/1> <rs/imm>: Store quotient of register rs (0) or immediate (1) and register rd.
		AND,	// <rd> <0/1> <rs/imm>: Store bitwise and of register rs (0) or immediate (1) and register rd.
		OR,		// <rd> <0/1> <rs/imm>: Store bitwise or of register rs (0) or immediate (1) and register rd.
		SHIFT,	// <rd> <0/1> <rs/imm>: Store bitwise shift of register rd by register rs (0) or immediate (1).
		PUSH,	// <__> <0/1> <rs/imm>: Push the value in rs (0) or immediate (1) to the stack.
		POP,	// <rd> <___> <______>: Pop the top element of the stack and set register rd to it.
		CALL,	// <__> <0/1> <rs/imm>: Push pc to the stack and set it to the value in rs (0) or to pc + imm (1).
		RET,	// <__> <___> <______>: Pop the top element from the stack and set pc to it.
		MOVC,	// <rd> <0/1> <rs/imm> <cond>: Store value in register rs (0) or immediate (1) in register rd if condition register is 1.
	};

	enum Reg
	{
		r1,
		r2,
		r3,
		r4,
		cr,
		ar,
		dr,
		pc,
	};

	enum CondFlags
	{
		equal = 1 << 0,
		not_equal = 1 << 1,
		greater = 1 << 2,
		greater_equal = 1 << 3,
		less = 1 << 4,
		less_equal = 1 << 5,
		zero = 1 << 6,
		not_zero = 1 << 7,
	};

	inline int reg[8]{};
	inline int condReg{};
	inline unsigned short instruction{};
	inline std::stack<int> stack{};

	int cycle(void*);
	void decode();
}

#endif