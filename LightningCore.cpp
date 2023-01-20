#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

void Lightning::init(SDL_Point screenSize)
{
	windowSize = screenSize;
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	pixelSize = windowSize.x * windowSize.y * 3;
	pitch = pixelSize / windowSize.y;
	
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, windowSize.x, windowSize.y);
	pixels = new Uint8[pixelSize]{};

	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

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
	
	SDL_Log("%d", SDL_GetTicks());

	SDL_UpdateTexture(screen, NULL, pixels, pitch);
	SDL_RenderCopy(renderer, screen, NULL, NULL);

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	SDL_WaitThread(CPU::thread, NULL);

	SDL_DestroyTexture(screen);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}