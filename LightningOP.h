#ifndef LIGHTNING_OP
#define LIGHTNING_OP

#undef OUT

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <bitset>

#include "LightningCore.h"

namespace Lightning
{
	namespace OP
	{
		enum Opcode
		{
			SET,	// Rd 0   0   imm : Set register Rd to 11-bit sign-extended immediate.
			SIE,	// Rd Rs1 Rs2 0   : Set register Rd to 1 if the values in Rs1 and Rs2 are equal, else set to 0.
			SIG,	// Rd Rs1 Rs2 0   : Set register Rd to 1 if Rs1 > Rs2, else set to 0.
			SIL,	// Rd Rs1 Rs2 0   : Set register Rd to 1 if Rs1 < Rs2, else set to 0.
			ADD,	// Rd Rs1 Rs2 0   : Add registers Rs1 and Rs2 and store the result in Rd.
			SUB,	// Rd Rs1 Rs2 0   : Compute Rs1 - Rs2 and store the result in Rd.
			MUL,	// Rd Rs1 Rs2 0   : Multiply the lower 8 bits of Rs1 and Rs2 and store the result in Rd.
			AND,	// Rd Rs1 Rs2 0   : Store in register Rd the bitwise AND of Rs1	and Rs2.
			OR,		// Rd Rs1 Rs2 0   : Store in register Rd the bitwise OR of Rs1 and Rs2.
			XOR,	// Rd Rs1 Rs2 0   : Store in register Rd the bitwise XOR of Rs1	and Rs2.
			NOT,	// Rd Rs1 0   0   : Store in register Rd the bitwise complement	Rs1.
			WMEM,	// 0  Rs1 Rs2 0   : Store to memory at address Rs2 the value in Rs1.
			RMEM,	// Rd 0   Rs2 0   : Load the word value at memory address Rs2 into Rd.
			JP,		// 0  0   0   imm : Set the PC to PC + 11-bit sign-extended immediate.
			JEQZ,	// 0  Rs1 0   imm : Set the PC to PC + 11-bit sign-extended immediate if Rs1 is zero.
			JNEZ,	// 0  Rs1 0   imm : Set the PC to PC + 11-bit sign-extended immediate if Rs1 is non-zero.
			CALL,	// 0  Rs1 0   0   : Push next address to the stack and jump to Rs1.
			RET,	// 0  0   0   0   : Pop top element of the stack and jump to it.
			HALT,	// 0  0   0   0   : Terminate the programme, freeing up allocated resources.

			total_opcodes,
		};

		std::vector<std::function<void()>> operations{};

		void loadOperations();
		void processOperation();
	}
}

#endif