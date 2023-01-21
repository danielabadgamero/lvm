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
		CPU::MOVR, CPU::r2, CPU::pc, 0,
		CPU::IN, CPU::r1, CPU::monitor, 2,
		CPU::CMPI, CPU::r1, 0, 1,
		CPU::MVRC, CPU::pc, CPU::r2, CPU::not_equal,
		CPU::OUTI, CPU::monitor, 0, 0,
		CPU::OUTI, CPU::monitor, 1, 0xff,
		CPU::OUTI, CPU::monitor, 2, 0,

		CPU::MOVR, CPU::r2, CPU::pc, 0,
		CPU::IN, CPU::r1, CPU::monitor, 2,
		CPU::CMPI, CPU::r1, 0, 1,
		CPU::MVRC, CPU::pc, CPU::r2, CPU::not_equal,
		CPU::OUTI, CPU::monitor, 0, 3,
		CPU::OUTI, CPU::monitor, 1, 0xff,
		CPU::OUTI, CPU::monitor, 2, 0,
		
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