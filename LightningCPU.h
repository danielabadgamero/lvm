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
		int running : 1{};
		int equal : 1{};
		int not_equal : 1{};
		int greater : 1{};
		int greater_equal : 1{};
		int less : 1{};
		int less_equal : 1{};
		int padding : 1{};
	} bistables{};

	int cycle(void*);
}

#endif