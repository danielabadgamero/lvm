#include "Dev.h"
#include "VM.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <iostream>
#include <unordered_map>

namespace Console
{
	void print_char()
	{
		std::cout << VM::RAM[0] << std::flush;
	}

	void print_string()
	{
		std::cout << VM::RAM << std::flush;
	}

	void print_stringln()
	{
		std::cout << VM::RAM << std::endl;
	}
	
	void new_line()
	{
		std::cout << std::endl;
	}

	void map(int func)
	{
		switch (func)
		{
		case 0x00: print_char(); break;
		case 0x01: print_string(); break;
		case 0x02: print_stringln(); break;
		case 0x03: new_line(); break;
		}
	}
}

namespace Graphics
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Color bg_color{};
	inline SDL_Color fg_color{};
	inline bool windowShouldClose{ true };
	inline TTF_Font* font{};
	inline SDL_DisplayMode screen{};

	struct Label
	{
		std::string text{};
		SDL_Point pos{};
		bool alignX{};
		bool alignY{};
	};

	inline std::unordered_map<unsigned char, Label> labels{};

	void init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		TTF_Init();
		SDL_GetCurrentDisplayMode(0, &screen);
		window = SDL_CreateWindow((char*)VM::RAM, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen.w, screen.h, SDL_WINDOW_BORDERLESS);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

		std::string title{ (char*)VM::RAM };
		bg_color.r = VM::RAM[title.size() + 1];
		bg_color.g = VM::RAM[title.size() + 2];
		bg_color.b = VM::RAM[title.size() + 3];
		bg_color.a = 0xff;
		fg_color.r = ~bg_color.r;
		fg_color.g = ~bg_color.g;
		fg_color.b = ~bg_color.b;
		fg_color.a = 0xff;

		font = TTF_OpenFont("/usr/share/fonts/gnu-free/FreeSans.otf", 18);

		windowShouldClose = false;
	}

	void events()
	{
		SDL_Event e{};
		while (SDL_PollEvent(&e))
			switch (e.type)
			{
			case SDL_QUIT:
				windowShouldClose = true;
				break;
			}
	}

	void draw()
	{
		SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
		SDL_RenderClear(renderer);

		for (const auto& l : labels)
		{
			SDL_Surface* s{ TTF_RenderText_Solid(font, l.second.text.c_str(), fg_color) };
			SDL_Texture* t{ SDL_CreateTextureFromSurface(renderer, s) };
			SDL_FreeSurface(s);
			SDL_Rect rect{ (l.second.pos.x * screen.w) / 256, (l.second.pos.y * screen.h) / 256, 0, 0 };
			SDL_QueryTexture(t, NULL, NULL, &rect.w, &rect.h);
			if (l.second.alignX) rect.x -= rect.w / 2;
			if (l.second.alignY) rect.y -= rect.h / 2;
			SDL_RenderCopy(renderer, t, NULL, &rect);
			SDL_DestroyTexture(t);
		}

		SDL_RenderPresent(renderer);
	}

	void get_state()
	{
		VM::RAM[0] = windowShouldClose;
		int numKeys{};
		const Uint8* keystate{ SDL_GetKeyboardState(&numKeys) };
		for (int i{}; i != numKeys - 1; i++)
			VM::RAM[i + 1] = keystate[i + 1];
	}

	void quit()
	{
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		TTF_Quit();
	}

	void create_label()
	{
		Label label{};
		label.text = std::string{ (char*)VM::RAM };
		unsigned char id{ VM::RAM[label.text.size() + 1] };
		if (label.text.empty() && labels.contains(id))
		{
			labels.erase(id);
			return;
		}
		
		label.pos.x = VM::RAM[label.text.size() + 2];
		label.pos.y = VM::RAM[label.text.size() + 3];
		label.alignX = VM::RAM[label.text.size() + 4];
		label.alignY = VM::RAM[label.text.size() + 5];

		labels[id] = label;
	}

	void map(int func)
	{
		switch (func)
		{
		case 0x00: init(); break;
		case 0x01: events(); break;
		case 0x02: draw(); break;
		case 0x03: get_state(); break;
		case 0x04: quit(); break;
		case 0x05: create_label(); break;
		}
	}
}

void Dev::init()
{
	devices[CONSOLE] = Console::map;
	devices[GRAPHICS] = Graphics::map;
}
