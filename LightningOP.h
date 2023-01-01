#ifndef LIGHTNING_OP
#define LIGHTNING_OP

#undef IN

#include <functional>
#include <vector>

namespace Lightning
{
	namespace OP
	{
		enum Opcode
		{
			//		   00000 000   0000  0000      00000000'00000000
			HALT,	// op    0     0     0         0                  : Stop execution.
			SET,	// op    Rd    0     0         #                  : Set register Rd to 16-bit immediate.
			SEQ,	// op    Rd    Rs1   Rs2       0                  : Set register Rd to 1 if registers Rs1 and Rs2 are equal, to 0 otherwise.
			SEQI,	// op    Rd    Rs1   0         #                  : Set register Rd to 1 if register Rs1 is equal to 16-bit immediate, to 0 otherwise.
			SGTI,	// op    Rd    Rs1   0         #                  : Set register Rd to 1 if register Rs1 is greater than 16-bit immediate, to 0 otherwise.
			ADD,	// op    Rd    Rs1   Rs2       0                  : Set register Rd to the sum of the registers Rs1 and Rs2.
			ADDI,	// op    Rd    Rs1   0         #                  : Set register Rd to the sum of the register Rs1 and the 16-bit immediate.
			MUL,	// op    Rd    Rs1   Rs2       0                  : Set register Rd to the product of the registers Rs1 and Rs2.
			MULI,	// op    Rd    Rs1   0         #                  : Set register Rd to the product of the register Rs1 and the 16-bit immediate.
			CALL,	// op    0     Rs1   0         0                  : Push the next instruction's address to the stack and jump to address in register Rs1.
			RET,	// op    0     0     0         0                  : Pop the top element of the stack and jump to it.
			JP,		// op    0     0     0         #                  : Set PC to PC + 16-bit immediate.
			JT,		// op    0     Rs1   0         #                  : Set PC to PC + 16-bit immediate if register Rs1 is non-zero.
			JF,		// op    0     Rs1   0         #                  : Set PC to PC + 16-bit immediate if register Rs1 is zero.
			RMEM,	// op    Rd    Rs1   0         0                  : Set register Rd to the 32-bit value at the address in register Rs1.
			WMEM,	// op    0     Rs1   Rs2       0                  : Set value at address in register Rs2 to value in register Rs1.
			COUT,	// op    0     Rs1   0         0                  : Print the ASCII value in register Rs1 to the console.
			COUTI,	// op    0     0     0         #                  : Print the ASCII value of the 16-bit immediate to the console.
			IOUT,	// op    0     Rs1   0         0                  : Print the integer value in register Rs1 to the console.
			IN,     // op    Rd    0     0         0                  : Get user input and store the value in register Rd.

			total_opcodes,
		};

		inline constexpr unsigned int opcode{ 0b11111000'00000000'00000000'00000000 };
		inline constexpr unsigned int Rd	{ 0b00000111'00000000'00000000'00000000 };
		inline constexpr unsigned int Rs1	{ 0b00000000'11110000'00000000'00000000 };
		inline constexpr unsigned int Rs2	{ 0b00000000'00001111'00000000'00000000 };
		inline constexpr unsigned int imm	{ 0b00000000'00000000'11111111'11111111 };

		struct
		{
			short opcode{};
			int* Rd{};
			int* Rs1{};
			int* Rs2{};
			short imm{};
		} instruction{};;

		inline std::vector<std::function<void()>> operations{};

		void loadOperations();
		void processOperation();
	}
}

#endif