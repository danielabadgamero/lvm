#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"
#include "LightningMonitor.h"
#include "LightningDisk.h"

void Lightning::init(SDL_Point screenSize)
{
	windowSize = screenSize;
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	Monitor::pixelSize = windowSize.x * windowSize.y * 3;
	Monitor::pitch = Monitor::pixelSize / windowSize.y;

	Monitor::screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, windowSize.x, windowSize.y);
	Monitor::pixels = new Uint8[Monitor::pixelSize]{};

	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);
	Disk::thread = SDL_CreateThread(Disk::cycle, "Disk", NULL);

	SDL_ShowCursor(SDL_DISABLE);
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
				CPU::bistables[CPU::running] = 0;
				break;
			}
			break;
		case SDL_QUIT:
			CPU::bistables[CPU::running] = 0;
			break;
		}

	Monitor::refresh();
}

void Lightning::quit()
{
	SDL_WaitThread(CPU::thread, NULL);
	SDL_WaitThread(Disk::thread, NULL);

	SDL_DestroyTexture(Monitor::screen);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}