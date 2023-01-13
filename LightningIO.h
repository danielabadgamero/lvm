#ifndef LIGHTNING_IO
#define LIGHTNING_IO

#include <SDL.h>

namespace IO
{
	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif