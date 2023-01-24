#include <SDL.h>
#include <iostream>

#include "LightningCore.h"
#include "LightningMonitor.h"

void Lightning::Monitor::refresh()
{
	SDL_memcpy(pixels, RAM + VIDEO, *rSize);

	SDL_UpdateTexture(screen, NULL, pixels, *rPitch);
	SDL_RenderCopy(renderer, screen, NULL, NULL);

	SDL_RenderPresent(renderer);
}