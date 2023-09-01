#include "Dev.h"
#include "VM.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <iostream>

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

	void init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		TTF_Init();
		SDL_DisplayMode screen{};
		SDL_GetCurrentDisplayMode(0, &screen);
		window = SDL_CreateWindow((char*)VM::RAM, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen.w, screen.h, SDL_WINDOW_BORDERLESS);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

		std::string title{ (char*)VM::RAM };
		bg_color.r = VM::RAM[title.size() + 1];
		bg_color.g = VM::RAM[title.size() + 2];
		bg_color.b = VM::RAM[title.size() + 3];
		bg_color.a = 0xff;

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
		SDL_RenderPresent(renderer);
	}

	void get_state()
	{
		VM::RAM[0] = windowShouldClose;
	}

	void quit()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		TTF_Quit();
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
		}
	}
}

void Dev::init()
{
	devices[CONSOLE] = Console::map;
	devices[GRAPHICS] = Graphics::map;
}
