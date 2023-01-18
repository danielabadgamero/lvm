#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point windowSize{};

	inline bool running{};

	inline int RAM[1 << 20]{};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif