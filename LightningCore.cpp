#include <SDL.h>

#include "LightningCPU.h"
#include "LightningCore.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GetCurrentDisplayMode(0, &screen);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_ShowCursor(SDL_DISABLE);

	Threads::CPU = SDL_CreateThread(CPU::cycle, "CPU", NULL);

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
	
	if (systemBus.control.chipSelect == 1)
	{
		if (systemBus.control.read)
		{
			systemBus.data = RAM[systemBus.address];
			systemBus.control.read = 0;
		}
		else if (systemBus.control.write)
		{
			RAM[systemBus.address] = systemBus.data;
			systemBus.control.write = 0;
		}
	}
	else
	{
		if (systemBus.control.read)
		{
			systemBus.data = ROM[systemBus.address];
			systemBus.control.read = 0;
		}
	}
}

void Lightning::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}