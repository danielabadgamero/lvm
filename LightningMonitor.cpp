#include <SDL.h>

#include "LightningCore.h"
#include "LightningMonitor.h"

void Lightning::Monitor::refresh()
{
	VRAM[*rAddr] = static_cast<unsigned char>(*rData);
	*rStatus = 1;

	SDL_memcpy(pixels, VRAM, static_cast<size_t>(windowSize.x) * windowSize.y * 3);

	SDL_UpdateTexture(screen, NULL, pixels, pitch);
	SDL_RenderCopy(renderer, screen, NULL, NULL);

	SDL_RenderPresent(renderer);
}