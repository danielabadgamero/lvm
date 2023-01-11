#ifndef LIGHTNING_GUI
#define LIGHTNING_GUI

#include <SDL.h>

namespace Lightning
{
	namespace GUI
	{
		inline SDL_Rect taskBarRect{};
		inline SDL_Color taskBarColor{ 0x40, 0x40, 0x80 };

		void drawGui();
		void draw();
	}
}

#endif