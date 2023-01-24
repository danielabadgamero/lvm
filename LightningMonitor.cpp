#include <SDL.h>
#include <iostream>

#include "LightningCore.h"
#include "LightningMonitor.h"

void Lightning::Monitor::refresh()
{
	SDL_memcpy(pixels, RAM + VIDEO, *rSize);

	std::cout << std::hex << (int*)RAM << '\n';

	SDL_UpdateTexture(screen, NULL, pixels, *rPitch);
	SDL_RenderCopy(renderer, screen, NULL, NULL);

	SDL_RenderPresent(renderer);
}