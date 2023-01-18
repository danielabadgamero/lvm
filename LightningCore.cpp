#include <SDL.h>

#include "LightningCore.h"

void Lightning::init(SDL_Point screenSize)
{
	windowSize = screenSize;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_ShowCursor(SDL_DISABLE);
	running = true;
}

void Lightning::cycle()
{
	while (SDL_PollEvent(&e))
		switch (e.type)
		{
		case SDL_KEYDOWN:
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				running = false;
				break;
			}
			break;
		case SDL_QUIT:
			running = false;
			break;
		}

	SDL_SetRenderDrawColor(renderer, 0x14, 0x14, 0x28, 0xff);
	SDL_RenderClear(renderer);



	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}