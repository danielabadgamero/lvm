#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

#include <bitset>

inline SDL_Window* window{};
inline SDL_Renderer* renderer{};
inline SDL_Event e{};
inline SDL_DisplayMode screen{};
inline SDL_Texture* monitor{};
inline Uint8* pixels{};

inline int pixelsSize{};
inline int pixelsPitch{};

inline constexpr int SYSCALLS	{ 0x000000 };
inline constexpr int VIDEO		{ 0xA11000 };

namespace Lightning::Core
{
	inline bool running{};

	inline unsigned int RAM[1 << 24]{};	// 16MB virtual
	inline constexpr unsigned int ROM[1 << 12]	// 4KB virtual
	{
		0b00001'1'00'10100001'00010000'00000001,
		0b00011'1'00'00000000'00000000'11111111,
		0b00100'1'00'00000000'00000000'00000010,
	};

	struct Bus
	{
		unsigned int data{};
		unsigned int address{};

		std::bitset<3> control{};
	};

	inline Bus systemBus{};

	enum
	{
		chipSelect,
		read,
		write,
	};

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