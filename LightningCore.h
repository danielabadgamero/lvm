#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline constexpr int WIDTH { 1200 };
	inline constexpr int HEIGHT{ 675 };

	inline constexpr int VIDEO{ 0x01000 };

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point screen[WIDTH * HEIGHT]{};

	inline bool running{};

	inline int RAM[1 << 20]{};

	void init();
	void cycle();
	void quit();
}

#endif