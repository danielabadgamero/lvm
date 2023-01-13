#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "LightningCore.h"
#include "LightningCPU.h"
#include "LightningIO.h"

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);
	SDL_GetDesktopDisplayMode(0, &screen);

	font = TTF_OpenFont("lucon.ttf", 20);

	CPU::thread = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	for (int i{}; i != 127; i++)
	{
		SDL_Surface* surface{ TTF_RenderGlyph_Blended(font, static_cast<Uint16>(i), {0xff, 0xff, 0xff, 0xff})};
		SDL_Texture* texture{ SDL_CreateTextureFromSurface(renderer, surface) };
		glyphs.push_back(texture);
		SDL_FreeSurface(surface);
	}

	SDL_Texture* texture{ IMG_LoadTexture(renderer, "cursor.png") };
	glyphs.push_back(texture);

	SDL_ShowCursor(SDL_DISABLE);
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
	{
		SDL_Rect charPos{};
		switch (mode)
		{
		case TXT:
			charPos = { i % screen.w, static_cast<int>(i / screen.w), 0, 20 };

			TTF_GlyphMetrics(font, RAM[VIDEO_TXT + i], NULL, NULL, NULL, NULL, &charPos.w);
			charPos.x *= charPos.w;
			charPos.y *= charPos.h;

			SDL_RenderCopy(renderer, glyphs.at(static_cast<uint64_t>(RAM[VIDEO_TXT + i])), NULL, &charPos);
			break;
		case GPH:
			break;
		}
	}

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	TTF_CloseFont(font);

	for (auto& glyph : glyphs)
		SDL_DestroyTexture(glyph);

	SDL_WaitThread(CPU::thread, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}