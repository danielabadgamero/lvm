#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <stack>

#undef OUT

inline constexpr char IR{ 0 }; // Instruction register
inline constexpr char AR{ 1 }; // Address register
inline constexpr char DR{ 2 }; // Data register
inline constexpr char AC{ 3 }; // Accumulator register

inline constexpr char R0{ 4 }; // General purpose register 0
inline constexpr char R1{ 5 }; // General purpose register 1
inline constexpr char R2{ 6 }; // General purpose register 2
inline constexpr char R3{ 7 }; // General purpose register 3
inline constexpr char R4{ 8 }; // General purpose register 4
inline constexpr char R5{ 9 }; // General purpose register 5
inline constexpr char R6{ 10 }; // General purpose register 6
inline constexpr char R7{ 11 }; // General purpose register 7

inline constexpr char CT{ 12 }; // Character temp register
inline constexpr char TR{ 13 }; // Test register
inline constexpr char LR{ 14 }; // Loop address register

inline constexpr char P0{ 15 }; // Function parameter register
inline constexpr char P1{ 16 }; // Function parameter register
inline constexpr char P2{ 17 }; // Function parameter register
inline constexpr char RT{ 18 }; // Return value register

inline constexpr int Rd		{ 0b00000000'11111111'00000000'00000000 };
inline constexpr int Rs1	{ 0b00000000'00000000'11111111'00000000 };
inline constexpr int Rs2	{ 0b00000000'00000000'00000000'11111111 };
inline constexpr int imm8	{ 0b00000000'00000000'00000000'11111111 };
inline constexpr int imm16	{ 0b00000000'00000000'11111111'11111111 };
inline constexpr int imm24	{ 0b00000000'11111111'11111111'11111111 };

enum
{
	dir,
	iso,
	exe,
};

namespace Lightning
{
	inline struct CPU
	{
		long long REG[24]{};
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
			CIN,	//	Rd	0	0	Get a character from the user into Rd.
			COUT,	//	0	Rs	0	Print the ASCII value in Rs.
			COUTI,	//	#	#	#	Print 3 ASCII values of each 8-bit immediate.
			INC,	//	Rd  #   #   Increment Rd by 16-bit immediate.
			RMEM,	//	0	0	0	Set data register to value at memory in address register.
			WMEM,	//	0	0	0	Set value at memory in address register to data register.
			RFS,	//	0	0	0	Set data register to value at filesystem in address register.
			WFS,	//	0	0	0	Set value at filesystem in address register to data register.
			JMP,	//	0	Rs	0	Jump to address Rs.
			JPI,	//	#	#	#	Increse PC by 24-bit immediate times 4 forwards.
			JPZ,	//	0	Rs1	Rs2	Jump to address Rs1 if Rs2 is zero.
			JPZI,	//	0	Rs1	#	Increase PC by immediate times 4 forwards if Rs1 is zero.
			JNZ,	//	0	Rs1	Rs2	Jump to address Rs1 if Rs2 is non-zero.
			JNZI,	//	0	Rs1	#	Increase PC by immediate times 4 forwards if Rs1 is non-zero.
			PUSH,	//	0	Rs1	0	Push Rs1 to the stack.
			POP,	//	Rd	0	0	Pop top address of the stack and set it to Rd.
			CALL,	//	0	Rs1	0	Push the next address to the stack and jump to address in Rs1.
			RET,	//	0	0	0	Pop top address of the stack and jump to it.

			CPY,	//	Rd	Rs1	0	Set Rd to Rs1.
			ADD,	//	Compute Rs1 + Rs2 and store the result in Rd.
			SUB,	//	Compute Rs1 - Rs2 and store the result in Rd.
			unused1,
			MUL,	//	Compute Rs1 * Rs2 and store the result in Rd.
			DIV,	//	Compute Rs1 / Rs2 and store the result in Rd.
			MOD,	//	Compute Rs1 % Rs2 and store the result in Rd.
			AND,	//	Compute Rs1 & Rs2 and store the result in Rd.
			OR,		//	Compute Rs1 | Rs2 and store the result in Rd.
			XOR,	//	Compute Rs1 ^ Rs2 and store the result in Rd.
			NOT,	//	Compute complement of Rs1 and store the result in Rd.
			SEQ,	//	Compute Rs1 == Rs2 and store the result in Rd.
			SGT,	//	Compute Rs1 > Rs2 and store the result in Rd.
			SLT,	//	Compute Rs1 < Rs2 and store the result in Rd.
			LSFT,	//	Compute Rs1 << Rs2 and store the result in Rd.
			RSFT,	//	Compute Rs1 >> Rs2 and store the result in Rd.
		};

		void process();
	} CPU{};

	inline std::stack<long long> stack{};
	inline char RAM[1 << 20]{}; // 1MB
	inline constexpr char ROM[1 << 10] // 1024 B
	{
		CPU::INC,	R1,		0,		32,		// skip file name
		CPU::SET,	R0,		0,		26,		// end of file character

		CPU::CPY,	AR,		R1,		0,		// load fs address
		CPU::RFS,	0,		0,		0,		// read next byte
		CPU::INC,	R1,		0,		1,		// jump to next byte in fs
		CPU::SEQ,	TR,		DR,		R0,		// check if end of file
		CPU::CPY,	AR,		R2,		0,		// load RAM address
		CPU::WMEM,	0,		0,		0,		// write into RAM
		CPU::INC,	R2,		0,		1,		// jump to next byte in RAM
		CPU::JPZI,	0,		TR,		-7,		// loop if not end of file
	};

	inline bool running{ true };

	void init();
}

#endif