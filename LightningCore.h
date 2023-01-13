#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Event e{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	void init();
	void cycle();
	void quit();
}

#endif