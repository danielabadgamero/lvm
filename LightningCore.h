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
		CPU::MOVI, CPU::r1, 0, 0,
		CPU::SWI, CPU::r1, 0, CPU::OUT,
		CPU::ADDI, CPU::r1, 0, 1,
		CPU::SWI, CPU::r1, 0, CPU::pc,
		CPU::ADDI, CPU::r1, 0, 1,
	};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif