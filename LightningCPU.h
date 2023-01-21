#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <bitset>

#include <SDL.h>

namespace Lightning::CPU
{
	inline SDL_Thread* thread;

	enum Opcode
	{	// 1		2	3	4
		HALT,
		MOVR,	//	rd	rs	0
		MOVI,	//	rd	#	#
		MVRC,	//	rd	rs1	b
		LWR,	//	rd	rs	0
		LWI,	//	rd	#	#
		SWR,	//	rd	rs	0
		SWI,	//	rd	0	#
		ADD,	//	rd	rs	0
		ADDI,	//	rd	#	#
		LSHIFT,	//	rd	#	#
		RSHIFT,	//	rd	#	#
		IN,		//	rd	p	r
		OUT,	//	p	r	rs
		OUTI,	//	p	r	#
		CMP,	//	0	rs	rs
		CMPI,	//	rs	#	#
	};

	enum Reg
	{
		r1,
		r2,
		r3,
		r4,
		ar,
		dr,
		ir,
		pc
	};

	enum Bistable
	{
		running,
		equal,
		not_equal,
		greater,
		greater_equal,
		less,
		less_equal,
	};

	enum Peripheral
	{
		monitor,
		disk,

		total_peripherals
	};

	inline int reg[8]{};

	inline int stack[1 << 16]{}; // 64 KB
	inline int sb{};

	inline int peripherals[total_peripherals][8]{};

	inline std::bitset<7> bistables{};

	int cycle(void*);
	void decode();
}

#endif