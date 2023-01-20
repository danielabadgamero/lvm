#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

namespace Lightning::CPU
{
	inline SDL_Thread* thread;

	enum Opcode
	{
		HALT,	// HALT
		MOV,	// MOV	000 11111111
	};

	enum Reg
	{
		r1,	// general purpose regiser 1
		r2,	// general purpose regiser 2
		r3,	// general purpose regiser 3
		r4,	// general purpose regiser 4
		  
		ar,	// address register
		dr,	// data register
		  
		ir,	// instruction register
		pc,	// programme counter register
	};

	inline int reg[8]{};

	inline int stack[1 << 16]{}; // 64 KB
	inline int sb{};

	struct
	{
		char running : 1{};
	} bistables{};

	int cycle(void*);
}

#endif