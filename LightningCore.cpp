#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"
#include "LightningCPU.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	VGA::thread = SDL_CreateThread(VGA::cycle, "VGA", NULL);
	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	RAM[0] = CPU::SET;
	RAM[1] = CPU::R0;
	RAM[2] = 'a';
	RAM[3] = CPU::SET;
	RAM[4] = CPU::R1;
	RAM[5] = 0;
	RAM[6] = CPU::SET;
	RAM[7] = CPU::R2;
	RAM[8] = 0;
	RAM[9] = CPU::CALL;
	RAM[10] = PRINT_CH;

	RAM[PRINT_CH + 0] = CPU::SET;
	RAM[PRINT_CH + 1] = CPU::MAR;
	RAM[PRINT_CH + 2] = FONT_CH;
	RAM[PRINT_CH + 3] = CPU::SUB;
	RAM[PRINT_CH + 4] = CPU::R0;
	RAM[PRINT_CH + 5] = ' ';
	RAM[PRINT_CH + 6] = CPU::MUL;

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
		}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawPoints(renderer, screen, WIDTH * HEIGHT);

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	SDL_WaitThread(VGA::thread, NULL);
	SDL_WaitThread(CPU::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}