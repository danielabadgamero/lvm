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

	inline std::vector<std::string> flags
	{
		"TRUE",
		"EQUAL",
		"NOT_EQUAL",
		"LESS",
		"GREATER",
		"ZERO",
		"NOT_ZERO",
		"CARRY",
	};

	inline std::vector<std::string> regs
	{
		"a",
		"b",
		"c",
		"d",
	};

	inline std::stack<long long> stack{};
	inline std::bitset<8> flag{};
	inline long long pc{};
	inline long long pb{};
	inline long long reg[4]{};
	inline char RAM[1 << 16]{};
	inline char disk[1 << 16][512]{};

	inline struct Op
	{
		unsigned char opcode : 4;
		unsigned char dest : 1;
		unsigned char destDir : 1;
		unsigned char src : 1;
		unsigned char srcDir : 1;

		void operator=(char);
	} op{};
	
	void reset();
	void loop();
}

#endif