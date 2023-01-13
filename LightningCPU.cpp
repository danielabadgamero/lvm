#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	while (!running);

	while (running)
	{
		RAM[VIDEO_TXT] = RAM[KEY_CHAR];
	}

	return 0;
}