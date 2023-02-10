#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"
#include "LightningMonitor.h"
#include "LightningDisk.h"

void Lightning::init()
{
	windowSize = { 960, 640 };
	SDL_Init(SDL_INIT_EVERYTHING);

	Disk::init();

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	*Monitor::rSize = windowSize.x * windowSize.y * 3;
	*Monitor::rPitch = *Monitor::rSize / windowSize.y;

	Monitor::screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, windowSize.x, windowSize.y);
	Monitor::pixels = new Uint8[*Monitor::rSize]{};

	Disk::thread = SDL_CreateThread(Disk::cycle, "Disk", NULL);
	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

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
				running = false;
				break;
			}
			break;
		case SDL_QUIT:
			running = false;
			break;
		}

	/*
	* Control register (cr) values:
	* 
	* 0: nothing
	* 1: read ROM
	* 2: read RAM
	* 3: write RAM
	* 
	*/

	switch (CPU::reg[CPU::cr])
	{
	case 1:
		CPU::reg[CPU::dr] = ROM[CPU::reg[CPU::ar]];
		break;
	case 2:
		CPU::reg[CPU::dr] = RAM[CPU::reg[CPU::ar]];
		break;
	case 3:
		RAM[CPU::reg[CPU::ar]] = CPU::reg[CPU::dr];
		break;
	default:
		CPU::reg[CPU::dr] = peripherals[CPU::reg[CPU::cr - 4]][CPU::ar];
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