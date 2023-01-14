#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "LightningCore.h"
#include "LightningCPU.h"

void Lightning::init(SDL_Point screenSize)
{
	windowSize = screenSize;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	TTF_Font* font{ TTF_OpenFont("lucon.ttf", height) };

	TTF_GlyphMetrics(font, 'a', NULL, NULL, NULL, NULL, &advance);

	for (int i{}; i != 127; i++)
	{
		SDL_Surface* surface{ TTF_RenderGlyph32_Blended(font, static_cast<char>(i), {0xff, 0xff, 0xff}) };
		glyphs[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	glyphs[127] = IMG_LoadTexture(renderer, "cursor.png");

	cursorPos = VIDEO;

	TTF_CloseFont(font);
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
			case SDL_SCANCODE_BACKSPACE:
				RAM[cursorPos] = 0;
				if (cursorPos > 0)
					cursorPos--;
				break;
			case SDL_SCANCODE_LSHIFT:
			case SDL_SCANCODE_RSHIFT:
			case SDL_SCANCODE_LCTRL:
			case SDL_SCANCODE_RCTRL:
			case SDL_SCANCODE_LALT:
			case SDL_SCANCODE_RALT:
				break;
			default:
				RAM[cursorPos] = static_cast<char>(e.key.keysym.sym);
				if (SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
					RAM[cursorPos] -= 0x20;
				cursorPos++;
				break;
			}
			break;
		}

	RAM[cursorPos] = 127;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);

	for (int i{}; i != (windowSize.x / advance) * (windowSize.y / height); i++)
	{
		SDL_Rect glyph{ ((i * advance) % windowSize.x), ((i * advance) / windowSize.x) * height, advance, height };
		SDL_RenderCopy(renderer, glyphs[RAM[VIDEO + i]], NULL, &glyph);
	}

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	for (int i{}; i != 128; i++)
		SDL_DestroyTexture(glyphs[i]);

	SDL_WaitThread(CPU::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}