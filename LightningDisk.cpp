#include "LightningCore.h"
#include "LightningCPU.h"
#include "LightningDisk.h"

void Lightning::Disk::init()
{
	SDL_memcpy(HDD[0], boot, 512);
}

int Lightning::Disk::cycle(void*)
{
	while (true)
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