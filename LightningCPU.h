#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <bitset>

namespace Lightning::CPU
{
	enum Opcode
	{
		//			Description (addrMode = 0)			Description (addrMode = 1)
		
		//	Data management
		MOV,	//	Load reg op2 to rDest				Load imm to rDest
		LD,		//	Load reg addr op2 to rDest			Load addr op2 to rDest
		ST,		//	Store reg op2 in addr rDest			Store imm op2 in addr rDest
		
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
		JMPC,	//	Set pc to reg op2 if flag rDest		Set pc to imm op2 if flag rDest

		//	Arithmeitc
		ADD,	//	Add reg op2 to rDest				Add imm op2 to rDest
		SUB,	//	Substract reg op2 from rDest		Substract imm op2 from rDest
		MUL,	//	Multiply reg op2 times rDest		Multiply imm op2 times rDest
		DIV,	//	Divide reg op2 by rDest				Divide imm op2 by rDest
		MOD,	//	Modulo of rDest by reg op2			Modulo of rDest by imm op2
		RSFT,	//	Shift rDest to the right by reg op2	Shift rDest to the right by imm op2
		LSFT,	//	Shift rDest to the left by reg op2	Shift rDest to the left by imm op2
	};

	inline int reg[4]{};
	
	inline unsigned int pc{};
	inline unsigned int pb{};

	enum CompFlags
	{
		equal,
		zero,
		greater,
		greater_equal,
		less,
		less_equal,

		total_flags
	};

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

	void decode();
	int cycle(void*);
}

#endif