#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"
#include "LightningCPU.h"
#include "LightningCHAR.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	VGA::thread = SDL_CreateThread(VGA::cycle, "VGA", NULL);
	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	CHAR::loadFont();

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
	RAM[11] = CPU::JMP;
	RAM[12] = 11;

	RAM[PRINT_CH + 0] = CPU::SET;
	RAM[PRINT_CH + 1] = CPU::MAR;
	RAM[PRINT_CH + 2] = FONT_CH;
	RAM[PRINT_CH + 3] = CPU::SUB;
	RAM[PRINT_CH + 4] = CPU::R0;
	RAM[PRINT_CH + 5] = ' ';
	RAM[PRINT_CH + 6] = CPU::MUL;
	RAM[PRINT_CH + 7] = CPU::R0;
	RAM[PRINT_CH + 8] = 32;
	RAM[PRINT_CH + 9] = CPU::ADD;
	RAM[PRINT_CH + 10] = CPU::MAR;
	RAM[PRINT_CH + 11] = CPU::R0;
	RAM[PRINT_CH + 12] = CPU::SET;
	RAM[PRINT_CH + 13] = CPU::R3;
	RAM[PRINT_CH + 14] = VIDEO;
	RAM[PRINT_CH + 15] = CPU::RMEM;
	RAM[PRINT_CH + 16] = CPU::MDR;
	RAM[PRINT_CH + 17] = CPU::MAR;
	RAM[PRINT_CH + 18] = CPU::WMEM;
	RAM[PRINT_CH + 19] = CPU::R3;
	RAM[PRINT_CH + 20] = CPU::MDR;
	RAM[PRINT_CH + 21] = CPU::ADD;
	RAM[PRINT_CH + 22] = CPU::MAR;
	RAM[PRINT_CH + 23] = 1;
	RAM[PRINT_CH + 24] = CPU::ADD;
	RAM[PRINT_CH + 25] = CPU::R3;
	RAM[PRINT_CH + 26] = 1;
	RAM[PRINT_CH + 27] = CPU::ADD;
	RAM[PRINT_CH + 28] = CPU::R4;
	RAM[PRINT_CH + 29] = 1;
	RAM[PRINT_CH + 30] = CPU::CMP;
	RAM[PRINT_CH + 31] = CPU::R4;
	RAM[PRINT_CH + 32] = 32;
	RAM[PRINT_CH + 33] = CPU::JLT;
	RAM[PRINT_CH + 34] = PRINT_CH + 15;
	RAM[PRINT_CH + 35] = CPU::RET;

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