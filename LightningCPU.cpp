#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	while (!running);

	while (running)
	{
		RAM[VIDEO_TXT] = SDL_GetKeyName(RAM[KEY_PRESS])[0];
	}

	return 0;
}