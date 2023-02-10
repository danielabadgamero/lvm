#ifndef LIGHTNING_DISK
#define LIGHTNING_DISK

#include <SDL.h>

#include "LightningCore.h"

namespace Lightning::Disk
{
	inline SDL_Thread* thread{};

	inline unsigned char HDD[1 << 20][512]{};	// 512 MB

	inline unsigned char boot[512]
	{
	};

	inline int* rSec{ &peripherals[disk][0] };
	inline int* rAddr{ &peripherals[disk][1] };
	inline int* rRead{ &peripherals[disk][2] };
	inline int* rWrite{ &peripherals[disk][3] };

	void init();
	int cycle(void*);
}

#endif