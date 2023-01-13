#include <SDL.h>
#include <SDL_ttf.h>

#include "LightningCore.h"
#include "LightningVGA.h"

int Lightning::VGA::LightningMain(void*)
{
	TTF_Init();

	TTF_Font* font{ TTF_OpenFont("lucon.ttf", 80) };

	while (Lightning::running)
	{
		SDL_RenderClear(renderer);

		for (int i{}; i != screen.w * screen.h; i++)
		{
			SDL_Surface* charSurface{ TTF_RenderText_Solid(font, &RAM[VIDEO_TXT + i], {0xff, 0xff, 0xff, 0xff}) };
			SDL_Texture* charTexture{ SDL_CreateTextureFromSurface(renderer, charSurface) };
			SDL_Rect charPos{ i % screen.w, static_cast<int>(i / screen.w), 25, 80 };

			SDL_RenderCopy(renderer, charTexture, NULL, &charPos);
		}

		SDL_RenderPresent(renderer);
	}

	return 0;
}