#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point windowSize{};
	inline SDL_Texture* screen{};
	inline Uint8* pixels{};

	inline int pixelSize{};
	inline int pitch{};

	inline bool running{};

	inline int RAM[1 << 20]{};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif