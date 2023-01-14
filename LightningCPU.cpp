#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	while (!running);

	while (running)
	{
		RAM[VIDEO_TXT] = RAM[KEY_STATE + SDL_SCANCODE_A] * 97;
	}

	return 0;
}