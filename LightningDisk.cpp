#include "LightningCore.h"
#include "LightningCPU.h"
#include "LightningDisk.h"

int Lightning::Disk::cycle(void*)
{
	while (~CPU::bistables[CPU::running]);

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
}