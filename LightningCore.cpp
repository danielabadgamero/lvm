#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

void Lightning::init(SDL_Point screenSize)
{
	windowSize = screenSize;
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	screen = new SDL_Point[windowSize.x * windowSize.y];

	VGA::thread = SDL_CreateThread(VGA::cycle, "VGA", NULL);

	SDL_ShowCursor(SDL_DISABLE);
	running = true;
}

int index{};

void Lightning::cycle()
{
	while (SDL_PollEvent(&e))
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		}

	RAM[VIDEO_VGA + index] = 1;
	index++;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawPoints(renderer, screen, windowSize.x * windowSize.y);

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	SDL_WaitThread(VGA::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}