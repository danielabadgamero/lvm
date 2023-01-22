#include "LightningCore.h"
#include "LightningCPU.h"
#include "LightningDisk.h"

void Lightning::Disk::init()
{
	SDL_memcpy(HDD[0], boot, 512);
	SDL_memcpy(HDD[1], kernel, 512);
	SDL_memcpy(HDD[2], draw_pixel, 512);
	SDL_memcpy(HDD[3], draw_line, 512);
}

int Lightning::Disk::cycle(void*)
{
	while (~CPU::bistables[CPU::running])
	{
		if (*rRead)
		{
			SDL_memcpy(&RAM[*rAddr], HDD[*rSec], 512);
			*rRead = false;
		}
		if (*rWrite)
		{
			SDL_memcpy(HDD[*rSec], &RAM[*rAddr], 512);
			*rWrite = false;
		}
	}

	while (CPU::bistables[CPU::running])
	{
		if (*rRead)
		{
			SDL_memcpy(&RAM[*rAddr], HDD[*rSec], 512);
			*rRead = false;
		}
		if (*rWrite)
		{
			SDL_memcpy(HDD[*rSec], &RAM[*rAddr], 512);
			*rWrite = false;
		}
	}

	return 0;
}