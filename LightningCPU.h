#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <bitset>

#include <SDL.h>

namespace Lightning::CPU
{
	inline SDL_Thread* thread;

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

	inline int reg[8]{};

	inline int stack[1 << 16]{}; // 64 KB
	inline int sb{};

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

	inline std::bitset<7> bistables{};

	int cycle(void*);
}

#endif