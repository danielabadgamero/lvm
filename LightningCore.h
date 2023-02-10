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
	inline bool running{};

	/*
	* Memory map:
	* 
	* 
	*/

	inline constexpr int VIDEO{ 0x100000 };

	inline short RAM[1 << 24]{};	// 16MB
	inline constexpr unsigned short ROM[1 << 12]	// 4KB
	{
		0b00000000'00000000,
	};


	enum Peripheral
	{
		monitor,
		disk,

		total_peripherals
	};
	inline int peripherals[total_peripherals][8]{};

	void init();
	void cycle();
	void quit();
}

#endif