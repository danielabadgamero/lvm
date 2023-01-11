#include "LightningCore.h"
#include "LightningGUI.h"
#include "LightningFS.h"

#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() }; (void)io;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	FS::loadFilesystem();

	running = true;
}

void Lightning::processEvents()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(window, &screen.w, &screen.h);
			break;
		}
	}
}

void Lightning::draw()
{
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui::NewFrame();

	GUI::drawGui();

	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	GUI::draw();

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	FS::saveFilesystem();

	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	window = nullptr;
	renderer = nullptr;

	SDL_Quit();
}