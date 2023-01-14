#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

namespace Lightning::CPU
{
	enum
	{

	};

	enum
	{

	};

	inline int REG[8]{};

	inline int PC{};
	inline int instruction{};

	inline SDL_Thread* thread{};

	int cycle(void*);
}

#endif