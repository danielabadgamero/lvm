#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_DisplayMode screen{};
	inline TTF_Font* font{};
	inline std::vector<SDL_Texture*> glyphs{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	constexpr inline unsigned int VIDEO_TXT{ 0x00001000 };
	constexpr inline unsigned int KEY_STATE{ 0x00200000 };
	constexpr inline unsigned int KEY_CHAR { 0x00200200 };

	void init();
	void cycle();
	void quit();
}

#endif