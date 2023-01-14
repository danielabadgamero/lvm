#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline constexpr int VIDEO{ 0x01000 };

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point windowSize{};
	inline SDL_Texture* glyphs[128]{};
	inline int advance{};
	inline constexpr int height{ 15 };
	inline int cursorPos{};

	inline bool running{};

	inline char RAM[1 << 20]{};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif