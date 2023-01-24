#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <bitset>

#include <SDL.h>

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
		MOV,	// <rd> <0/1> <rs/imm>: Store value in register rs (0) or immediate (1) in register rd.
		ADD,	// <rd> <0/1> <rs/imm>: Store addition of register rs (0) or immediate (1) and register rd.
		SUB,	// <rd> <0/1> <rs/imm>: Store substraction of register rs (0) or immediate (1) and register rd.
		MUL,	// <rd> <0/1> <rs/imm>: Store product of register rs (0) or immediate (1) and register rd.
		DIV,	// <rd> <0/1> <rs/imm>: Store quotient of register rs (0) or immediate (1) and register rd.
		AND,	// <rd> <0/1> <rs/imm>: Store bitwise and of register rs (0) or immediate (1) and register rd.
		OR,		// <rd> <0/1> <rs/imm>: Store bitwise or of register rs (0) or immediate (1) and register rd.
		SHIFT,	// <rd> <0/1> <rs/imm>: Store bitwise shift of register rd by register rs (0) or immediate (1).
		LW,		// <rd> <0/1> <rs/imm>: Store in register rd the value in memory at pc + imm (1) or at rs (0).
		SW,		// <rd> <0/1> <rs/imm>: Store in memory at address rd value in rs (0) or immediate (0).
		PUSH,	// <__> <0/1> <rs/imm>: Push the value in rs (0) or immediate (1) to the stack.
		POP,	// <rd> <___> <______>: Pop the top element of the stack and set register rd to it.
		CALL,	// <__> <0/1> <rs/imm>: Push pc to the stack and set it to the value in rs (0) or to pc + imm (1).
		RET,	// <__> <___> <______>: Pop the top element from the stack and set pc to it.
		MOVC,	// <rd>
	};

	enum Reg
	{
		r1,
		r2,
		r3,
		r4,
		r5,
		ar,
		dr,
		pc,
	};

	enum CondFlags
	{
		running = 1 << 0,
		equal = 1 << 1,
		not_equal = 1 << 2,
		greater = 1 << 3,
		greater_equal = 1 << 4,
		less = 1 << 5,
		less_equal = 1 << 6,
		zero = 1 << 7,
		not_zero = 1 << 8,
	};

	enum Peripheral
	{
		monitor,
		disk,

		total_peripherals
	};

	inline int reg[8]{};
	inline int condReg{};
	inline unsigned short instruction{};

	inline int stack[1 << 16]{}; // 64 KB
	inline int sb{};

	inline int peripherals[total_peripherals][8]{};

	int cycle(void*);
	void decode();
}

#endif