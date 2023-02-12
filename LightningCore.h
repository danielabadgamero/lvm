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

	inline char RAM[1 << 16]{};	// 64KB
	inline constexpr unsigned char ROM[1 << 12]	// 4KB
	{
	};

	inline struct
	{
		short data : 16{};
	} dataBus{};

	inline struct
	{
		short address : 16{};
	} addressBus{};

	inline struct
	{
		char chipSelectRAM : 1{};
		char chipSelectROM : 1{};
		char read : 1{};
		char write : 1{};
		char byteSize : 1{};
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