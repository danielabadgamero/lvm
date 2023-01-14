#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

#include <vector>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_DisplayMode screen{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	void init();
	void cycle();
	void quit();
}

#endif