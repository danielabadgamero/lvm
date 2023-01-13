#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);

	VGA::thread = SDL_CreateThread(VGA::LightningMain, "VGA", nullptr);

	running = true;
}

void Lightning::cycle()
{

}

void Lightning::quit()
{
	SDL_WaitThread(VGA::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}