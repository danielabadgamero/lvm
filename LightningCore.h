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

	inline char RAM[1 << 24]{};	// 16MB
	inline constexpr unsigned char ROM[1 << 12]	// 4KB
	{
		CPU::MOVI, CPU::r1, 0, 1,
		CPU::OUT, CPU::disk, 2, CPU::r1,

		CPU::MOVR, CPU::r1, CPU::pc, 0,
		CPU::IN, CPU::r2, CPU::disk, 2,
		CPU::CMPI, CPU::r2, 0, 1,
		CPU::MVRC, CPU::pc, CPU::r1, CPU::equal,
	};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif