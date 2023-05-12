#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <vector>
#include <string>

namespace Lightning
{
	inline std::vector<std::string> opcodes
	{
		// Data management
		"MOV",
		"PUSH",
		"POP",
		"IN",
		"OUT",
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

	inline long long reg[4]{};
	inline char RAM[1 << 16]{};
	inline char disk[1 << 16][512]{};
	
	void loop();
}

#endif