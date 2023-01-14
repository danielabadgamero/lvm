#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

namespace Lightning::CPU
{
	constexpr inline unsigned long long R0{ 0 };
	constexpr inline unsigned long long R1{ 1 };
	constexpr inline unsigned long long R2{ 2 };
	constexpr inline unsigned long long R3{ 3 };

	constexpr inline unsigned long long IR{ 4 };
	constexpr inline unsigned long long PC{ 5 };

	constexpr inline unsigned long long AR{ 6 };
	constexpr inline unsigned long long DR{ 7 };
	constexpr inline unsigned long long AC{ 8 };

	inline constexpr int Rd		{ 0b00000000'11111111'00000000'00000000 };
	inline constexpr int Rs1	{ 0b00000000'00000000'11111111'00000000 };
	inline constexpr int Rs2	{ 0b00000000'00000000'00000000'11111111 };
	inline constexpr int imm8	{ 0b00000000'00000000'00000000'11111111 };
	inline constexpr int imm16	{ 0b00000000'00000000'11111111'11111111 };
	inline constexpr int imm24	{ 0b00000000'11111111'11111111'11111111 };

	enum Opcode
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
		unused,
		ADD,	//	Compute Rs1 + Rs2 and store the result in Rd.
		SUB,	//	Compute Rs1 - Rs2 and store the result in Rd.
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

	inline int REG[8]{};
	inline int* stack{};
	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif