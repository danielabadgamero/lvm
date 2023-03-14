#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <stack>

namespace Lightning::CPU
{
	enum CompFlag
	{
		equal,
		not_equal,
		greater,
		greater_equal,
		less,
		less_equal,

		total_compFlags
	};

	enum Opcode
	{
		//			Description (addrMode == 0)			Description (addrMode == 1)
		UNUSED_1,

		//	Data management
		MOV,	//	Load reg op2 to rDest				Load imm op2 to rDest
		SPB,	//	Set programme base to reg op2		Set programme base to imm op2
		PSHB,	//	Push programme base to stack		Push programme base to stack
		POPB,	//	Pop programme base from stack		Pop programme base from stack
		LD,		//	Load reg addr op2 to rDest			Load addr op2 to rDest
		ST,		//	Store reg op2 in addr rDest			Store imm op2 in addr rDest
		PUSH,	//	Push reg op2 to stack				Push imm op2 to stack
		POP,	//	Pop top of stack into rDest			Pop top of stack into rDest
		IN,		//	TBD: communication with peripherals.
		OUT,	//	TBD: communication with peripherals.

		//	Flow control
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

		//	Arithmetic
		ADD,	//	Add reg op2 to rDest				Add imm op2 to rDest
		SUB,	//	Substract reg op2 from rDest		Substract imm op2 from rDest
		MUL,	//	Multiply reg op2 times rDest		Multiply imm op2 times rDest
		DIV,	//	Divide reg op2 by rDest				Divide imm op2 by rDest
		MOD,	//	Modulo of rDest by reg op2			Modulo of rDest by imm op2
		SHFT,	//	Shift rDest to the left by reg op2	Shift rDest to the left by imm op2

		//	Logic
		AND,	//	Bitwise AND of rDest and reg op2	Bitwise AND of rDest and imm op2
		OR,		//	Bitwise OR of rDest and reg op2		Bitwise OR of rDest and imm op2
		XOR,	//	Bitwise XOR of rDest and reg op2	Bitwise XOR of rDest and imm op2
		NOT,	//	Bitwise neg of reg op2 in rDest		Bitwise neg of imm op2 in rDest
	};

	inline short reg[4]{};

	inline bool compFlags[total_compFlags]{};

	inline union
	{
		struct
		{
			unsigned short op2 : 8;			// Second operand. Interpretation depends on adrMode
			unsigned short reg : 2;			// 4 general purpose registers
			unsigned short addrMode : 1;	// Opcode-specific addressing mode modifier
			unsigned short opcode : 5;		// 32 different opcodes
		} bitfields;

		unsigned short instruction{};
	} ir{};

	inline unsigned short pc{};
	inline unsigned short pb{};

	inline std::stack<short> stack{};

	void decode();
	int cycle(void*);
}

#endif