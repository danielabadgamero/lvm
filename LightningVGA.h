#ifndef LIGHTNING_VGA
#define LIGHTNING_VGA

#include <SDL.h>

namespace Lightning
{
	namespace VGA
	{
		SDL_Thread* thread{};

		int LightningMain(void*);
	}
}

#endif