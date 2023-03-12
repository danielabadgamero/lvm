#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <bitset>
#include <stack>

typedef void (*interrupt)();

namespace Lightning::CPU
{
	enum Opcode
	{
		//			Description (addrMode == 0)			Description (addrMode == 1)

		//	Data management
		MOV,	//	Load reg op2 to rDest				Load imm op2 to rDest
		SPB,	//	Set pb register to reg op2			Set pb register to imm op2
		LD,		//	Load reg addr op2 to rDest			Load addr op2 to rDest
		ST,		//	Store reg op2 in addr rDest			Store imm op2 in addr rDest
		PUSH,	//	Push reg op2 to stack				Push imm op2 to stack
		POP,	//	Pop top of stack into rDest			Pop top of stack into rDest
		IN,		//	TBD: communication with peripherals.
		OUT,	//	TBD: communication with peripherals.

		//	Flow control
		//	value in rDest changes behaviour of pc change:
		//	rDest	Desc
		//	00		Absolute address
		//	01		Relative to pc (programme counter)
		//	10		Relative to pb (programme base)
		//	11		Absolute indirect (2 jumps)
		HALT,	//	Finish execution					Switch to chip in op2
		CMP,	//	Eval rDest and reg op2 to flags		Eval rDest and imm op2 to flags
		JMP,	//	Set pc to reg op2					Set pc to imm op2
		JEQ,	//	Set pc to reg op2 if equal flag		Set pc to imm op2 if equal flag
		JNE,	//	Set pc to reg op2 if not equal flag	Set pc to imm op2 if not equal flag
		JGT,	//	Set pc to reg op2 if greater flag	Set pc to imm op2 if greater flag
		JGE,	//	Set pc to reg op2 if grt eq flag	Set pc to imm op2 if grt eq flag
		JLT,	//	Set pc to reg op2 if less flag		Set pc to imm op2 if less flag
		JLE,	//	Set pc to reg op2 if less eq flag	Set pc to imm op2 if less eq flag
		CALL,	//	Push pc to stack and set to reg op2	Push pc to stack and set to imm op2
		RET,	//	Pop top of stack and jump to it		Pop top of stack and jump to it
		INT,	//	Call from interrupt table. Arguments in stack, guaranteed to remain unchanged

		//	Arithmetic
		ADD,	//	Add reg op2 to rDest				Add imm op2 to rDest
		SUB,	//	Substract reg op2 from rDest		Substract imm op2 from rDest
		MUL,	//	Multiply reg op2 times rDest		Multiply imm op2 times rDest
		DIV,	//	Divide reg op2 by rDest				Divide imm op2 by rDest
		MOD,	//	Modulo of rDest by reg op2			Modulo of rDest by imm op2
		SHFT,	//	Shift rDest to the right by reg op2	Shift rDest to the right by imm op2

		//	Logic
		AND,	//	Bitwise AND of rDest and reg op2	Bitwise AND of rDest and imm op2
		NAND,	//	Bitwise NAND of rDest and reg op2	Bitwise NAND of rDest and imm op2
		OR,		//	Bitwise OR of rDest and reg op2		Bitwise OR of rDest and imm op2
		XOR,	//	Bitwise XOR of rDest and reg op2	Bitwise XOR of rDest and imm op2
		NOR,	//	Bitwise NOR of rDest and reg op2	Bitwise NOR of rDest and imm op2
		NOT,	//	Bitwise neg. of reg op2 in rDest	Bitwise neg. of imm op2 in rDest
	};

	inline int reg[4]{};

	inline unsigned int pc{};
	inline unsigned int pb{};
	
	enum CompFlags
	{
		equal,
		not_equal,
		greater,
		greater_equal,
		less,
		less_equal,

		total_flags
	};

	enum Reg
	{
		ax,
		bx,
		cx,
		dx,
	};

	inline struct
	{
		int x{};
		int y{};
	} cursor;

	enum Interrupts
	{
		// int name			args
		print_char,		//	ax: char
		new_line,		//	-
		read_disk,		//	ax: sector_start, bx: num_sectors, cx: mem_addr
		write_disk,		//	ax: mem_start, bx: num_sectors, cx: sector_addr

		total_interrupts
	};

	inline interrupt interruptTable[total_interrupts];

	inline std::bitset<total_flags> compFlags{};

	inline union
	{
		struct
		{
			unsigned int op2 : 24;		// second operand. Interpretation depends on adrMode
			unsigned int reg : 2;		// 4 general purpose registers
			unsigned int addrMode : 1;	// opcode-specific addressing mode modifier
			unsigned int opcode : 5;	// 32 different opcodes
		} bitfields;

		unsigned int instruction{};
	} ir{};

	inline std::stack<int> stack{};

	void decode();
	int cycle(void*);
}

#endif