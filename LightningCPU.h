#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <bitset>

#include <SDL.h>

namespace Lightning::CPU
{
	inline SDL_Thread* thread;

	/*
	* Instruction format:
	* 
	*/

	enum Opcode
	{

	};

	enum Reg
	{
		r1,
		r2,
		r3,
		r4,
		r5,
		ar,
		dr,
		pc,
	};

	enum CondFlags
	{
		running = 1 << 0,
		equal = 1 << 1,
		not_equal = 1 << 2,
		greater = 1 << 3,
		greater_equal = 1 << 4,
		less = 1 << 5,
		less_equal = 1 << 6,
		zero = 1 << 7,
		not_zero = 1 << 8,
	};

	enum Peripheral
	{
		monitor,
		disk,

		total_peripherals
	};

	inline int reg[8]{};
	inline int condReg{};
	inline unsigned short instruction{};

	inline int stack[1 << 16]{}; // 64 KB
	inline int sb{};

	inline int peripherals[total_peripherals][8]{};

	int cycle(void*);
	void decode();
}

#endif