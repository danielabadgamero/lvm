#include "LightningCore.h"
#include "LightningGUI.h"

#include <SDL.h>
#include <imgui.h>

void Lightning::GUI::drawGui()
{
}

void Lightning::GUI::draw()
{
	taskBarRect.h = static_cast<int>(screen.h * 0.05);
	taskBarRect.w = screen.w;
	taskBarRect.y = screen.h - taskBarRect.h;
	SDL_SetRenderDrawColor(renderer, taskBarColor.r, taskBarColor.g, taskBarColor.b, 0xff);
	SDL_RenderFillRect(renderer, &taskBarRect);
}