#include <SDL.h>
#include <SDL_ttf.h>

#include "LightningCore.h"
#include "LightningVGA.h"

int Lightning::VGA::LightningMain(void*)
{
	TTF_Init();

	TTF_Font* font{ TTF_OpenFont("lucon.ttf", 30) };

	while (Lightning::running)
	{
		SDL_RenderClear(renderer);

		for (int i{}; i != screen.w * screen.h; i++)
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

		SDL_RenderPresent(renderer);
	}

	return 0;
}