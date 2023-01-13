#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

namespace Lightning::CPU
{
	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif