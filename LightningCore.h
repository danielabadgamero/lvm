#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point windowSize{};


	inline char RAM[1 << 24]{};	// 16MB
	inline constexpr unsigned char ROM[1 << 12]	// 4KB
	{
		0b01000000, 0b00000000, 0b00010000, 0b00000000,
		0b01000001, 0b00000000, 0b00000000, 0b11111111,
		0b01001000, 0b00100000, 0b00000000, 0b00000000,

		0b01000000, 0b00000000, 0b00000000, 0b00000000,
		0b01000001, 0b00000000, 0b00000000, 0b01000111,
		0b01001000, 0b00100000, 0b00000000, 0b00000000,
	};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif