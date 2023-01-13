#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>
#include <SDL_ttf.h>

namespace Lightning
{
	inline enum
	{
		TXT,
		GPH
	} mode{};

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_DisplayMode screen{};
	inline TTF_Font* font{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	constexpr inline unsigned int VIDEO_TXT{ 0x1000 };

	void init();
	void cycle();
	void quit();
}

#endif