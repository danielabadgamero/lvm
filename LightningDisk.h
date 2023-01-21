#ifndef LIGHTNING_DISK
#define LIGHTNING_DISK

#include <SDL.h>

#include "LightningCPU.h"

namespace Lightning::Disk
{
	inline SDL_Thread* thread{};

	inline unsigned char HDD[1 << 20][512]{};	// 512 MB

	inline unsigned char boot[512]
	{
		CPU::MOVI, CPU::r1, 0, 0xff,
		CPU::OUT, CPU::monitor, 1, CPU::r1,
		
		CPU::MOVR, CPU::r1, CPU::pc, 0,
		CPU::MOVR, CPU::pc, CPU::r1, 0,
	};

	inline int* rSec{ &CPU::peripherals[CPU::disk][0] };
	inline int* rAddr{ &CPU::peripherals[CPU::disk][1] };
	inline int* rRead{ &CPU::peripherals[CPU::disk][2] };
	inline int* rWrite{ &CPU::peripherals[CPU::disk][3] };

	void init();
	int cycle(void*);
}

#endif