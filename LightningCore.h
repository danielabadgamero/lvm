#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

inline SDL_Window* window{};
inline SDL_Renderer* renderer{};
inline SDL_Event e{};
inline SDL_DisplayMode screen{};

namespace Lightning::Core
{
	inline bool running{};

	inline unsigned int RAM[1 << 24]{};	// 64MB, 16MB virtual
	inline constexpr unsigned int ROM[1 << 12]	// 16KB, 4KB virtual
	{
	};

	inline struct
	{
		unsigned int data{};
		unsigned int address{};

		union
		{
			unsigned int chipSelect : 1;	// 0 for ROM, 1 for RAM
			unsigned int read : 1;
			unsigned int write : 1;
		} control{};
	} systemBus{};

	namespace Threads
	{
		inline SDL_Thread* Core{};
		inline SDL_Thread* CPU{};
	}

	void init();
	int cycle(void*);
	void quit();
}

#endif