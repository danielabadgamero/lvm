#include <SDL.h>

#include "LightningCPU.h"
#include "LightningCore.h"

void Lightning::Core::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GetCurrentDisplayMode(0, &screen);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_ShowCursor(SDL_DISABLE);

	Threads::CPU = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	monitor = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, screen.w, screen.h);
	pixelsSize = screen.w * screen.h * 3;
	pixelsPitch = screen.w * 3;
	pixels = new Uint8[pixelsSize]{};

	running = true;
}

int Lightning::Core::cycle()
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

	SDL_memcpy(pixels, RAM + VIDEO, pixelsSize);
	SDL_UpdateTexture(monitor, NULL, pixels, pixelsPitch);
	SDL_RenderCopy(renderer, monitor, NULL, NULL);

	SDL_RenderPresent(renderer);

	return 0;
}

void Lightning::Core::quit()
{
	SDL_WaitThread(Threads::CPU, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}