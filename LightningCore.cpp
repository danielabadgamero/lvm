#include <SDL.h>

#include "LightningCore.h"

void Lightning::init(SDL_Point windowSize)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);

	SDL_ShowCursor(SDL_DISABLE);
	running = true;
}

void Lightning::cycle()
{
	while (SDL_PollEvent(&e))
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		}

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}