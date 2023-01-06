#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <stack>

#undef OUT

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
inline constexpr unsigned char TR{ 13 }; // Test register
inline constexpr unsigned char LR{ 14 }; // Loop address register

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
			IOUT,	//	0	Rs	0	Print integer value in Rs.
			IOUTI,	//	#	#	#	Print 24-bit immediate.
			INC,	//	Rd  #   #   Increment Rd by 16-bit immediate.
			RMEM,	//	0	0	0	Set data register to value at memory in address register.
			WMEM,	//	0	0	0	Set value at memory in address register to data register.
			RFS,	//	0	0	#	Set data register to value at filesystem in address register in partition of 8-bit immediate.
			WFS,	//	0	0	#	Set value at filesystem in address register to data register in partition of 8-bit immediate.
			JMP,	//	0	Rs	0	Jump to address Rs.
			JPI,	//	#	#	#	Increse PC by 24-bit immediate times 4 forwards.
			JPZ,	//	0	Rs1	Rs2	Jump to address Rs1 if Rs2 is zero.
			JPZI,	//	0	Rs1	#	Increase PC by immediate times 4 forwards if Rs1 is zero.
			JNZ,	//	0	Rs1	Rs2	Jump to address Rs1 if Rs2 is non-zero.
			JNZI,	//	0	Rs1	#	Increase PC by immediate times 4 forwards if Rs1 is non-zero.
			PUSH,	//	0	0	0	Push current address to the stack.
			POP,	//	0	0	0	Pop top address of the stack and set it to address register.
			CALL,	//	0	Rs1	0	Push the next address to the stack and jump to address in Rs1.
			RET,	//	0	0	0	Pop top address of the stack and jump to it.

			CPY,	//	Rd	Rs1	0	Set Rd to Rs1.
			ADD,	//	Compute Rs1 + Rs2 and store the result in Rd.
			SUB,	//	Compute Rs1 - Rs2 and store the result in Rd.
			MUL,	//	Compute Rs1 * Rs2 and store the result in Rd.
			DIV,	//	Compute Rs1 / Rs2 and store the result in Rd.
			unused1,
			unused2,
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
	inline unsigned char RAM[1 << 20]{}; // 1MB
	inline constexpr unsigned char ROM[1 << 15] // 32KB
	{
		//		Opcode				Rd / #		Rs1	/ #		Rs2 / #
		(unsigned char)CPU::JPI,	0,			0,			9,		// skip halt instruction
		(unsigned char)CPU::COUTI,	'N',		'o',		' ',	// print "No bootloader found\n"
		(unsigned char)CPU::COUTI,	'b',		'o',		'o',	// print "No bootloader found\n"
		(unsigned char)CPU::COUTI,	't',		'l',		'o',	// print "No bootloader found\n"
		(unsigned char)CPU::COUTI,	'a',		'd',		'e',	// print "No bootloader found\n"
		(unsigned char)CPU::COUTI,	'r',		' ',		'f',	// print "No bootloader found\n"
		(unsigned char)CPU::COUTI,	'o',		'u',		'n',	// print "No bootloader found\n"
		(unsigned char)CPU::COUTI,	'd',		'!',		'\n',	// print "No bootloader found\n"
		(unsigned char)CPU::HALT,	0,			0,			0,		// halt for further use
		
		(unsigned char)CPU::SET,	LR,			0,			1,		// set direction of halt instruction

		(unsigned char)CPU::JPI,	0,			0,			6,		// skip function definition
		(unsigned char)CPU::RFS,	0,			0,			0,		// read next byte of filesystem
		(unsigned char)CPU::SEQ,	TR,			DR,			R0,		// test byte
		(unsigned char)CPU::JPZ,	0,			LR,			TR,		// jump to halt
		(unsigned char)CPU::INC,	AR,			0,			1,		// jump to next byte
		(unsigned char)CPU::RET,	0,			0,			0,		// return

		(unsigned char)CPU::SET,	R1,			0,			11,		// address of check function

		(unsigned char)CPU::SET,	R0,			0,			1,		// check if first item is a file
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function
		(unsigned char)CPU::SET,	R0,			0,			29,		// check for separation character
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function
		(unsigned char)CPU::SET,	R0,			0,			'c',	// check for character
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function
		(unsigned char)CPU::SET,	R0,			0,			'o',	// check for character
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function
		(unsigned char)CPU::SET,	R0,			0,			'r',	// check for character
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function
		(unsigned char)CPU::SET,	R0,			0,			'e',	// check for character
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function
		(unsigned char)CPU::SET,	R0,			0,			29,		// check for end of file
		(unsigned char)CPU::CALL,	0,			R1,			0,		// call check function

		(unsigned char)CPU::CPY,	R1,			AR,			4,		// save fs address
		(unsigned char)CPU::SET,	R3,			0,			44,		// halt address
		
		(unsigned char)CPU::PUSH,	0,			0,			0,		// loop from here
		(unsigned char)CPU::CPY,	AR,			R1,			0,		// load fs address
		(unsigned char)CPU::RFS,	0,			0,			0,		// read next byte
		(unsigned char)CPU::INC,	R1,			0,			1,		// jump to next byte in fs
		(unsigned char)CPU::SEQ,	TR,			DR,			R0,		// check if end of file

		(unsigned char)CPU::CPY,	AR,			R2,			0,		// load RAM address
		(unsigned char)CPU::JNZ,	0,			R3,			TR,		// exit loop if end of file
		(unsigned char)CPU::WMEM,	0,			0,			0,		// write into RAM
		(unsigned char)CPU::INC,	R2,			0,			1,		// jump to next byte in RAM
		(unsigned char)CPU::POP,	0,			0,			0,		// pop address
		(unsigned char)CPU::JPZ,	0,			AR,			TR,		// loop if not end of file
		(unsigned char)CPU::POP,	0,			0,			0,		// pop and exit
	};

	inline bool running{ true };

	void init();
	void clearScreen();
}

#endif