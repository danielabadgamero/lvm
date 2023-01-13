#include <SDL.h>

#include "LightningCore.h"
#include "LightningIO.h"

int Lightning::IO::cycle(void*)
{
	while (!running);

	while (running)
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			RAM[KEY_PRESS] = static_cast<char>(e.key.keysym.sym);
			break;
		}
	}

	return 0;
}