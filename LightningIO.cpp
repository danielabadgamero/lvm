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
			RAM[KEY_PRESS + 1] = static_cast<char>(e.key.keysym.sym >> 8);
			RAM[KEY_PRESS + 2] = static_cast<char>(e.key.keysym.sym >> 16);
			RAM[KEY_PRESS + 3] = static_cast<char>(e.key.keysym.sym >> 24);
			RAM[KEY_MOD] = static_cast<char>(e.key.keysym.mod);
			RAM[KEY_CHAR] = RAM[KEY_PRESS];
			if ((e.key.keysym.mod == KMOD_LSHIFT || e.key.keysym.mod == KMOD_RSHIFT) && RAM[KEY_CHAR] <= 'z' && RAM[KEY_CHAR] >= 'a')
				RAM[KEY_CHAR] -= 0x20;
			if (RAM[KEY_CHAR] < 0)
				RAM[KEY_CHAR] = 0;
			break;
		}
	}

	return 0;
}