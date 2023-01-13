#include <SDL.h>

#include "LightningCore.h"
#include "LightningIO.h"

int IO::cycle(void* data)
{
	char* RAM{ (char*)data };
	while (Lightning::running)
	{
		switch (Lightning::e.type)
		{
		case SDL_KEYDOWN:
			RAM[Lightning::KEY_PRESS] = static_cast<char>(Lightning::e.key.keysym.sym);
			break;
		}
	}

	return 0;
}