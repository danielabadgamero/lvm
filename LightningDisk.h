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
		// Load kernel
		CPU::OUTI, CPU::disk, 0, 1,
		CPU::MOVI, CPU::r1, (KERNEL & 0xff00) >> 8, KERNEL & 0xff,
		CPU::OUT, CPU::disk, 1, CPU::r1,
		CPU::OUTI, CPU::disk, 2, 1,

		CPU::MOVR, CPU::ar, CPU::pc, 0,
		CPU::IN, CPU::r2, CPU::disk, 2,
		CPU::CMPI, CPU::r2, 0, 1,
		CPU::MVCR, CPU::pc, CPU::ar, CPU::equal,

		// Load draw_pixel
		CPU::OUTI, CPU::disk, 0, 2,
		CPU::MOVI, CPU::r1, (DRAW_PIXEL & 0xff00) >> 8, DRAW_PIXEL & 0xff,
		CPU::OUT, CPU::disk, 1, CPU::r1,
		CPU::OUTI, CPU::disk, 2, 1,

		CPU::MOVR, CPU::ar, CPU::pc, 0,
		CPU::IN, CPU::r2, CPU::disk, 2,
		CPU::CMPI, CPU::r2, 0, 1,
		CPU::MVCR, CPU::pc, CPU::ar, CPU::equal,

		// Load draw_line
		CPU::OUTI, CPU::disk, 0, 3,
		CPU::MOVI, CPU::r1, (DRAW_LINE & 0xff00) >> 8, DRAW_LINE & 0xff,
		CPU::OUT, CPU::disk, 1, CPU::r1,
		CPU::OUTI, CPU::disk, 2, 1,

		CPU::MOVR, CPU::ar, CPU::pc, 0,
		CPU::IN, CPU::r2, CPU::disk, 2,
		CPU::CMPI, CPU::r2, 0, 1,
		CPU::MVCR, CPU::pc, CPU::ar, CPU::equal,

		// Call kernel
		CPU::CALL, 0, (KERNEL & 0x00ff00) >> 8, KERNEL & 0x0000ff,


		CPU::MOVR, CPU::ar, CPU::pc, 0,
		CPU::MOVR, CPU::pc, CPU::ar, 0
	};

	inline unsigned char kernel[512]
	{

	};

	inline unsigned char draw_pixel[512]
	{

	};

	inline unsigned char draw_line[512]
	{

	};

	inline int* rSec{ &CPU::peripherals[CPU::disk][0] };
	inline int* rAddr{ &CPU::peripherals[CPU::disk][1] };
	inline int* rRead{ &CPU::peripherals[CPU::disk][2] };
	inline int* rWrite{ &CPU::peripherals[CPU::disk][3] };

	void init();
	int cycle(void*);
}

#endif