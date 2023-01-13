#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Event e{};
	SDL_DisplayMode screen{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	constexpr inline unsigned int VIDEO_TXT{ 0x1000 };

	void init();
	void cycle();
	void quit();
}

#endif