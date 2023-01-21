#ifndef LIGHTNING_DISK
#define LIGHTNING_DISK

#include <SDL.h>

#include "LightningCPU.h"

namespace Lightning::Disk
{
	inline SDL_Thread* thread{};

	inline int* rAddr{ &CPU::peripherals[CPU::disk][0] };
	inline int* rData{ &CPU::peripherals[CPU::disk][1] };
	inline int* rRead{ &CPU::peripherals[CPU::disk][2] };

	inline unsigned char HDD[1 << 21][512]{};	// 1GB

	int cycle(void*);
}

#endif