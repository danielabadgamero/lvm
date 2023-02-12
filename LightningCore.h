#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_DisplayMode screen{};
	inline bool running{};

	inline short RAM[1 << 16]{};	// 16MB
	inline constexpr unsigned short ROM[1 << 12]	// 4KB
	{
	};

	inline struct
	{
		int data : 32{};
	} dataBus{};

	inline struct
	{
		int address : 32{};
	} addressBus{};

	inline struct
	{
		char chipSelectRAM : 1{};
		char chipSelectROM : 1{};
	} controlBus{};

	namespace Threads
	{
		inline SDL_Thread* CPU{};
	}

	void init();
	void cycle();
	void quit();
}

#endif