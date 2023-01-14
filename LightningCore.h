#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline constexpr int VIDEO_VGA{ 0x00000000 };

	inline constexpr int WIDTH { 600 };
	inline constexpr int HEIGHT{ 300 };

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Rect screen[WIDTH * HEIGHT]{};

	inline bool running{};

	inline char RAM[(1 << 16) - 1]{};
	inline int REG[8]{};

	void init();
	void cycle();
	void quit();
}

#endif