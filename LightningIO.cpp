#include <SDL.h>

#include "LightningCore.h"
#include "LightningIO.h"

int Lightning::IO::cycle(void*)
{
	while (!running);
	
	SDL_StartTextInput();
	
	while (running)
	{
		const Uint8* keyState{ SDL_GetKeyboardState(NULL) };
		SDL_memcpy(RAM + KEY_STATE, keyState, 512);
		switch (e.type)
		{
		case SDL_TEXTINPUT:
			RAM[KEY_CHAR] = e.text.text[0];
			break;
		}
	}

	SDL_StopTextInput();

	return 0;
}