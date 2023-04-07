#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <stack>
#include <bitset>

namespace Lightning::CPU
{
	enum Opcode
	{
		MOV,

	};

	inline struct Instruction
	{
		unsigned char dAddr : 2;	//	destination address
		unsigned char opcode : 6;	//	64 different opcodes
		
		Instruction& operator=(unsigned char data)
		{
			opcode = (data & 0b1111'1100) >> 2;
			dAddr = (data & 0b0000'0011) >> 1;

			return *this;
		}
	} instruction{};

	inline short pc{};
	inline short regs[4]{};
	inline std::stack<short> stack{};
	inline std::bitset<2> flags{};
	enum
	{
		greater,
		equal
	};

	void decode();
	int cycle(void*);
}

#endif