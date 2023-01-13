#ifndef LIGHTNING_IO
#define LIGHTNING_IO

#include <SDL.h>

namespace Lightning::IO
{
	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif