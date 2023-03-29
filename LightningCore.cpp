#include <SDL.h>

#include "LightningCPU.h"
#include "LightningCore.h"

#include <fstream>
#include <filesystem>

void Lightning::Core::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GetCurrentDisplayMode(0, &screen);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_ShowCursor(SDL_DISABLE);

	Threads::CPU = SDL_CreateThread(CPU::cycle, "CPU", NULL);

	monitor = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, screen.w, screen.h);
	pixelsSize = screen.w * screen.h * 3;
	pixelsPitch = screen.w * 3;
	pixels = new Uint8[pixelsSize]{};
	videoSize = (screen.w / 24) * (screen.h / 32);
	videoPitch = screen.w / 24;

	if (std::filesystem::exists("fs"))
	{
		std::ifstream fs_in{ "fs", std::ios::binary };
		int sector{};
		while (!fs_in.eof())
		{
			fs_in.read(disk[sector], 512);
			sector++;
		}
	}

	running = true;
}

int Lightning::Core::cycle()
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

	for (int c{}; c != videoSize; c++)
		if (Core::RAM[c + VIDEO] >= ' ' && Core::RAM[c + VIDEO] <= '~')
			for (int i{}; i != 32; i++)
				for (int j{}; j != 3; j++)
					for (int k{}; k != 8; k++)
					{
						pixels[(c / videoPitch + i) * pixelsPitch + (c % videoPitch) * 72 + j * 24 + k * 3] = ((Core::font[Core::RAM[c + VIDEO] * 96 + i * 3 + j - ' ' * 96] & (1 << (7 - k))) >> (7 - k)) * 0xff;
						pixels[(c / videoPitch + i) * pixelsPitch + (c % videoPitch) * 72 + j * 24 + k * 3 + 1] = ((Core::font[Core::RAM[c + VIDEO] * 96 + i * 3 + j - ' ' * 96] & (1 << (7 - k))) >> (7 - k)) * 0xff;
						pixels[(c / videoPitch + i) * pixelsPitch + (c % videoPitch) * 72 + j * 24 + k * 3 + 2] = ((Core::font[Core::RAM[c + VIDEO] * 96 + i * 3 + j - ' ' * 96] & (1 << (7 - k))) >> (7 - k)) * 0xff;
					}

	SDL_UpdateTexture(monitor, NULL, pixels, pixelsPitch);
	SDL_RenderCopy(renderer, monitor, NULL, NULL);
	
	SDL_RenderPresent(renderer);

	return 0;
}

void Lightning::Core::quit()
{
	if (std::filesystem::exists("fs"))
		std::remove("fs");

	std::ofstream fs_out{ "fs", std::ios::binary };
	for (int i{}; i != (1 << 17); i++)
		fs_out.write(disk[i], 512);

	SDL_WaitThread(Threads::CPU, NULL);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}