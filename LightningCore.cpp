#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);

	SDL_GetDesktopDisplayMode(0, &screen);

	RAM[VIDEO_TXT] = 'H';
	RAM[VIDEO_TXT + 1] = 'e';
	RAM[VIDEO_TXT + 2] = 'l';
	RAM[VIDEO_TXT + 3] = 'l';
	RAM[VIDEO_TXT + 4] = 'o';
	RAM[VIDEO_TXT + 5] = ',';
	RAM[VIDEO_TXT + 6] = ' ';
	RAM[VIDEO_TXT + 7] = 'w';
	RAM[VIDEO_TXT + 8] = 'o';
	RAM[VIDEO_TXT + 9] = 'r';
	RAM[VIDEO_TXT + 10] = 'l';
	RAM[VIDEO_TXT + 11] = 'd';
	RAM[VIDEO_TXT + 12] = '!';

	VGA::thread = SDL_CreateThread(VGA::LightningMain, "VGA", nullptr);

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
}

void Lightning::quit()
{
	SDL_WaitThread(VGA::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}