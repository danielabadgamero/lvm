#include <SDL.h>

#include "LightningCore.h"

int main(int, char**)
{
	Lightning::Core::init();

	while (Lightning::Core::running)
	{
		while (SDL_PollEvent(&e))
			switch (e.type)
			{
			case SDL_KEYDOWN:
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					Lightning::Core::running = false;
					break;
				}
				break;
			case SDL_QUIT:
				Lightning::Core::running = false;
				break;
			}

		SDL_memcpy(pixels, Lightning::Core::RAM + VIDEO, pixelsSize);
		SDL_UpdateTexture(monitor, NULL, pixels, pixelsPitch);
		SDL_RenderCopy(renderer, monitor, NULL, NULL);

		SDL_RenderPresent(renderer);
	}

	Lightning::Core::quit();

	return 0;
}