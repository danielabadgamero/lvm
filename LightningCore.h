#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Rect screen{};

	inline bool running{};

	void init();
	void processEvents();
	void draw();
	void quit();
}

#endif