#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

#include "LightningCPU.h"

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point windowSize{};

	/*
	* Memory map:
	* 
	* 
	*/

	inline constexpr int VIDEO{ 0x100000 };

	inline char RAM[1 << 24]{};	// 16MB
	inline constexpr unsigned char ROM[1 << 12]	// 4KB
	{
		0b00000000, 0b00000000,
	};

	void init();
	void cycle();
	void quit();
}

#endif