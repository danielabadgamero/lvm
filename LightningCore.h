#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline constexpr int VIDEO{ 0x10000 };
	inline constexpr int PRINT_CH{ 0x00200 };	// R0: character, R1: row, R2: col

	inline constexpr int MAX_RAM{ 1 << 20 };

	inline constexpr int WIDTH { 600 };
	inline constexpr int HEIGHT{ 300 };

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