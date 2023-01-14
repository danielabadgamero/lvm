#ifndef LIGHTNING_VGA
#define LIGHTNING_VGA

#include <SDL.h>

namespace Lightning::VGA
{
	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif