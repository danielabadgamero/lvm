#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <vector>
#include <string>
#include <bitset>
#include <stack>

namespace Lightning
{
	inline std::vector<std::string> opcodes
	{
		// Data management
		"MOV",
		"PUSH",
		"POP",
		"RD",
		"WR",
		"REC",

		// Flow control
		"HALT",
		"JMP",
		"CALL",
		"RET",

		// Arithmetic and bitwise logic
		"ADD",
		"SUB",
		"MUL",
		"DIV",
		"AND",
		"OR",
	};

	enum Flag
	{
		TRUE,
		EQUAL,
		LESS,
		CARRY,
	};

	inline std::stack<long long> stack{};
	inline std::bitset<4> flags{};
	inline long long reg[4]{};
	inline char RAM[1 << 16]{};
	inline char disk[1 << 16][512]{};

	struct Instruction
	{
		union
		{
			char byte{};
			char sMode : 1;
			char dMode : 1;
			char dReg : 2;
			char opcode : 4;
		} op{};

		union
		{
			short byte{};
			char padding : 5;
			char dReg : 2;
			char sMode : 1;
		} regSrc{};
	} instr{};
	
	void reset();
	void loop();
}

#endif