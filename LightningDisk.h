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
		CPU::PSHI, 0xff, 0x00, 0x00,	// Color
		CPU::PSHI, 0, 0x00, 0x00,		// Y - Coordinate
		CPU::PSHI, 0, 0x00, 0x00,		// X - Coordinate
		CPU::CALL, 0, (DRAW_PIXEL & 0x00ff00) >> 8, DRAW_PIXEL & 0x0000ff,

		CPU::PSHI, 0x00, 0x00, 0xff,	// Color
		CPU::PSHI, 0, 0x02, 0x1c,		// Y - Coordinate start
		CPU::PSHI, 0, 0x03, 0xc0,		// X - Coordinate start
		CPU::PSHI, 0, 0x02, 0x1c,		// Y - Coordinate end
		CPU::PSHI, 0, 0x03, 0xc0,		// X - Coordinate end
		CPU::CALL, 0, (DRAW_LINE & 0x00ff00) >> 8, DRAW_LINE & 0x0000ff,

		CPU::RET, 0, 0, 0,
	};

	inline unsigned char draw_pixel[512]
	{
		CPU::POP, CPU::ar, 0, 0,

		CPU::PSHI, (VIDEO & 0xff0000) >> 16, (VIDEO & 0x00ff00) >> 8, VIDEO & 0x0000ff,
		CPU::POP, CPU::r1, 0, 0,
		CPU::POP, CPU::r2, 0, 0,
		CPU::MULI, CPU::r2, 0, 3,
		CPU::ADD, CPU::r1, CPU::r2, 0,
		CPU::POP, CPU::r2, 0, 0,
		CPU::IN, CPU::r3, CPU::monitor, 1,
		CPU::MUL, CPU::r2, CPU::r3, 0,
		CPU::ADD, CPU::r1, CPU::r2, 0,

		CPU::POP, CPU::r2, 0, 0,
		CPU::PSHR, CPU::r2, 0, 0,
		CPU::RSHIFT, CPU::r2, 0, 16,
		CPU::SWR, CPU::r1, CPU::r2, 0,
		CPU::ADDI, CPU::r1, 0, 1,

		CPU::POP, CPU::r2, 0, 0,
		CPU::PSHR, CPU::r2, 0, 0,
		CPU::RSHIFT, CPU::r2, 0, 8,
		CPU::SWR, CPU::r1, CPU::r2, 0,
		CPU::ADDI, CPU::r1, 0, 1,

		CPU::POP, CPU::r2, 0, 0,
		CPU::SWR, CPU::r1, CPU::r2, 0,

		CPU::PSHR, CPU::ar, 0, 0,

		CPU::RET,
	};

	inline unsigned char draw_line[512]
	{
		CPU::POP, CPU::ar, 0, 0,

		CPU::POP, CPU::op3, 0, 0,	// x - end
		CPU::POP, CPU::op4, 0, 0,	// y - end
		CPU::POP, CPU::op1, 0, 0,	// x - start
		CPU::POP, CPU::op2, 0, 0,	// y - start
		CPU::POP, CPU::r4, 0, 0,	// color



		CPU::PSHR, CPU::ar, 0, 0,

		CPU::RET,
	};

	inline int* rSec{ &CPU::peripherals[CPU::disk][0] };
	inline int* rAddr{ &CPU::peripherals[CPU::disk][1] };
	inline int* rRead{ &CPU::peripherals[CPU::disk][2] };
	inline int* rWrite{ &CPU::peripherals[CPU::disk][3] };

	void init();
	int cycle(void*);
}

#endif