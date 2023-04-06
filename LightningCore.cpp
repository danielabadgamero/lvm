#include <SDL.h>
#include <SDL_ttf.h>

#include "LightningCPU.h"
#include "LightningCore.h"

#include <fstream>
#include <filesystem>

void Lightning::Core::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_GetCurrentDisplayMode(0, &screen);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_ShowCursor(SDL_DISABLE);

	videoSize = (screen.w / 15) * (screen.h / 30);
	videoPitch = screen.w / 15;
	font = TTF_OpenFont("font.ttf", 24);

	running = true;
	CPU_Thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);
}

void Lightning::Core::cycle()
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
			default:
				RAM[2] = static_cast<unsigned char>(e.key.keysym.sym);
			}
			break;
		case SDL_QUIT:
			running = false;
			break;
		}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);

	for (int i{}; i != videoSize; i++)
	{
		SDL_Surface* surface{ TTF_RenderGlyph_Solid(font, static_cast<Uint16>(RAM[VIDEO + i]), { 0xff, 0xff, 0xff }) };
		SDL_Texture* glyph{ SDL_CreateTextureFromSurface(renderer, surface) };
		SDL_FreeSurface(surface);
		
		SDL_Rect glyphRect{ (i % videoPitch) * 15, (i / videoPitch) * 30, 15, 30 };
		SDL_RenderCopy(renderer, glyph, NULL, &glyphRect);
		SDL_DestroyTexture(glyph);
	}
	int cursor{ (RAM[0] << 8) | RAM[1] };
	SDL_Rect cursorRect{ (cursor % 128) * 15, (cursor / 128) * 30, 15, 30 };
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, &cursorRect);

	SDL_RenderPresent(renderer);
}

void Lightning::Core::quit()
{
	SDL_WaitThread(CPU_Thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
	TTF_Quit();
}