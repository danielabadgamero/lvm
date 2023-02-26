#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

namespace Lightning::CPU
{
	enum Opcode
	{
		//			Description (0)						Description (1)
		HALT,	//	Finish execution					Switch to chip in op2
		MOV,	//	Load reg op2 to rDest				Load imm to rDest
		LD,		//	Load reg addr op2 to rDest			Load addr op2 to rDest
		ST,		//	Store reg op2 in addr rDest			Store imm op2 in addr rDest
		JMP,	//	Set pc to reg op2					Set pc to imm op2
	};

	inline int reg[4]{};
	
	inline unsigned int pc{};

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