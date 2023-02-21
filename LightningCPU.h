#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

namespace Lightning::CPU
{
	enum Opcode
	{
		HALT,
		MOVI,
		MOVR,
	};

	inline int reg[4]{};
	
	inline unsigned int pc{};
	inline unsigned int mdr{};
	inline unsigned int mar{};

	inline union
	{
		unsigned int opcode : 5;	// 32 different opcodes
		unsigned int addrMode : 1;	// opcode-specific addressing mode modifier
		unsigned int reg : 2;		// 4 general purpose registers
		unsigned int op2 : 24;		// second operand. Interpretation depends on adrMode

		unsigned int instruction{};
	} ir{};

	void decode();
	int cycle(void*);
}

#endif