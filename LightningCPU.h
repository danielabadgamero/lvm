#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

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

	struct
	{
		char running : 1{};
		char equal : 1{};
		char not_equal : 1{};
		char equal : 1{};
		char greater : 1{};
		char greater_equal : 1{};
		char less : 1{};
		char less_equal : 1{};
	} bistables{};

	int cycle(void*);
}

#endif