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

	inline short RAM[1 << 24]{};	// 16MB
	inline constexpr unsigned short ROM[1 << 12]	// 4KB
	{
	};

	void init();
	void cycle();
	void quit();
}

#endif