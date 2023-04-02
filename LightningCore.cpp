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

	if (std::filesystem::exists("disk"))
	{
		std::ifstream fs_in{ "disk", std::ios::binary };
		int sector{};
		while (!fs_in.eof())
		{
			fs_in.read(disk[sector], 512);
			sector++;
		}
	}

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
			}
			break;
		case SDL_QUIT:
			running = false;
			break;
		}

	SDL_RenderClear(renderer);

	for (int i{}; i != videoSize; i++)
	{
		SDL_Surface* surface{ TTF_RenderGlyph_Solid(font, static_cast<Uint16>(Core::RAM[VIDEO + i]), { 0xff, 0xff, 0xff }) };
		SDL_Texture* glyph{ SDL_CreateTextureFromSurface(renderer, surface) };
		SDL_FreeSurface(surface);
		
		SDL_Rect glyphRect{ (i % videoPitch) * 15, (i / videoPitch) * 30, 15, 30 };
		SDL_RenderCopy(renderer, glyph, NULL, &glyphRect);
		SDL_DestroyTexture(glyph);
	}
	SDL_RenderPresent(renderer);
}

void Lightning::Core::quit()
{
	SDL_WaitThread(CPU_Thread, NULL);

	if (std::filesystem::exists("disk"))
		std::remove("disk");

	std::ofstream fs_out{ "disk", std::ios::binary };
	for (int i{}; i != (1 << 16); i++)
		fs_out.write(disk[i], 512);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
	TTF_Quit();
}