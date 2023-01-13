#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

int Lightning::VGA::LightningMain(void*)
{
	while (Lightning::running)
	{
		SDL_RenderClear(renderer);



		SDL_RenderPresent(renderer);
	}

	return 0;
}