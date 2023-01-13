#include <SDL.h>
#include <SDL_ttf.h>

#include "LightningCore.h"
#include "LightningCPU.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);
	SDL_GetDesktopDisplayMode(0, &screen);

	font = TTF_OpenFont("lucon.ttf", 30);

	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	running = true;
	mode = TXT;
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

	SDL_RenderClear(renderer);

	for (int i{}; i != screen.w * screen.h; i++)
		switch (mode)
		{
		case TXT:
		{
			const char character[2]{ RAM[VIDEO_TXT + i], 0 };
			SDL_Surface* charSurface{ TTF_RenderText_Solid(font, character, {0xff, 0xff, 0xff, 0xff}) };
			SDL_Texture* charTexture{ SDL_CreateTextureFromSurface(renderer, charSurface) };
			SDL_Rect charPos{ i % screen.w, static_cast<int>(i / screen.w) };

			TTF_SizeText(font, character, &charPos.w, &charPos.h);
			charPos.x *= charPos.w;
			charPos.y *= charPos.h;

			SDL_RenderCopy(renderer, charTexture, NULL, &charPos);

			SDL_DestroyTexture(charTexture);
			SDL_FreeSurface(charSurface);
		}
			break;
		case GPH:
			break;
		}

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	TTF_CloseFont(font);

	SDL_WaitThread(CPU::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}