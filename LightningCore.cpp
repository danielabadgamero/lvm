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

	if (controlBus.chipSelectRAM)
	{
		if (controlBus.read)
		{
			dataBus.data = RAM[addressBus.address];
			if (controlBus.byteSize)
			{
				dataBus.data <<= 8;
				dataBus.data += RAM[addressBus.address + 1];
			}
			controlBus.read = 0;
		}
		else if (controlBus.write)
		{
			if (controlBus.byteSize)
			{
				RAM[addressBus.address] = static_cast<unsigned char>(dataBus.data >> 8);
				RAM[addressBus.address + 1] = static_cast<unsigned char>(dataBus.data);
			}
			else
			{
				RAM[addressBus.address] = static_cast<unsigned char>(dataBus.data);
			}
			controlBus.write = 0;
		}
	}
	else if (controlBus.chipSelectROM)
	{
		if (controlBus.read)
		{
			dataBus.data = ROM[addressBus.address];
			if (controlBus.byteSize)
			{
				dataBus.data <<= 8;
				dataBus.data += RAM[addressBus.address + 1];
			}
			controlBus.read = 0;
		}
	}
}

void Lightning::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}