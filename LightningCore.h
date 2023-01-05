#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <stack>

inline constexpr unsigned char IR{ 0 }; // Instruction register (32-bit)
inline constexpr unsigned char AR{ 1 }; // Address register
inline constexpr unsigned char DR{ 2 }; // Data register
inline constexpr unsigned char AC{ 3 }; // Accumulator register

inline constexpr unsigned char R0{ 4 }; // General purpose register 0
inline constexpr unsigned char R1{ 5 }; // General purpose register 1
inline constexpr unsigned char R2{ 6 }; // General purpose register 2
inline constexpr unsigned char R3{ 7 }; // General purpose register 3
inline constexpr unsigned char R4{ 8 }; // General purpose register 4
inline constexpr unsigned char R5{ 9 }; // General purpose register 5
inline constexpr unsigned char R6{ 10 }; // General purpose register 6
inline constexpr unsigned char R7{ 11 }; // General purpose register 7

inline constexpr unsigned char ST{ 12 }; // Status register

inline constexpr int Rd		{ 0b00000000'11111111'00000000'00000000 };
inline constexpr int Rs1	{ 0b00000000'00000000'11111111'00000000 };
inline constexpr int Rs2	{ 0b00000000'00000000'00000000'11111111 };
inline constexpr int imm8	{ 0b00000000'00000000'00000000'11111111 };
inline constexpr int imm16	{ 0b00000000'00000000'11111111'11111111 };
inline constexpr int imm24	{ 0b00000000'11111111'11111111'11111111 };

namespace Lightning
{
	inline struct CPU
	{
		long long REG[8]{};
		long long PC{};

		struct ALU
		{
			long long process(int, long long, long long);
		} ALU{};

		enum
		{
			HALT,	//	0	0	0	Stop execution.
			SET,	//	Rd	#	#	Set Rd to 16-bit immediate.
			SAC,	//	#	#	#	Set accumulator register to 24-bit immediate.
			SAR,	//	#	#	#	Set address register to 24-bit immediate.
			SDR,	//	#	#	#	Set data register to 24-bit immediate.
			COUT,	//	0	Rs	0	Print the ASCII value in Rs.
			IOUT,	//	0	Rs	0	Print the integer value in Rs.
			RMEM,	//	0	0	0	Set data register to value at memory in address register.
			WMEM,	//	0	0	0	Set value at memory in address register to data register.

			CPY,	//	Rd	Rs1	0	Set Rd to Rs1.
			ADD,	//	Compute Rs1 + Rs2 and store the result in Rd.
			SUB,	//	Compute Rs1 - Rs2 and store the result in Rd.
			MUL,	//	Compute Rs1 * Rs2 and store the result in Rd.
			DIV,	//	Compute Rs1 / Rs2 and store the result in Rd.
			MOD,	//	Compute Rs1 % Rs2 and store the result in Rd.
			AND,	//	Compute Rs1 & Rs2 and store the result in Rd.
			OR,		//	Compute Rs1 | Rs2 and store the result in Rd.
			XOR,	//	Compute Rs1 ^ Rs2 and store the result in Rd.
			LSFT,	//	Compute Rs1 << Rs2 and store the result in Rd.
			RSFT,	//	Compute Rs1 >> Rs2 and store the result in Rd.
			NOT,	//	Compute complement of Rs1 and store the result in Rd.
		};

		void process();
	} CPU{};

	inline unsigned char* RAM{};
	inline constexpr unsigned char ROM[1 << 10]
	{
		//		Opcode				Rd / #			Rs1	/ #			Rs2 / #
	};

	inline bool running{ true };

	void init();
	void quit();
	void clearScreen();
}

#endif