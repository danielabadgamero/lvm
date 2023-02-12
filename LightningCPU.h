#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

namespace Lightning::CPU
{
	enum Opcode
	{
		HLT,
		MOV,
	};

	inline union
	{
		struct
		{
			char opcode : 6;	// 64 possible opcodes
			char dMode : 1;		// reg (4 bits) or address (16 bits)
			char sMode : 1;		// reg (4 bits) or address (16 bits)
		} fields;

		char bytes{};
	} instruction{};

	int cycle(void*);
}

#endif