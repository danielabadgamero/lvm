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
	inline bool chipSelected{}; // 0 for ROM, 1 for RAM

	inline unsigned char RAM[1 << 24]{};	// 16MB virtual
	inline constexpr unsigned char ROM[1 << 12]	// 4KB virtual
	{
		0b00000'1'00, 0xA1, 0x10, 0x00,
		0b00010'1'00, 0x00, 0x00, 0xFF,
		0b00111'1'00, 0x00, 0x00, 0x01,
		0b00101'1'00, 0x00, 0x00, 0x04,
	};

	namespace Threads
	{
		inline SDL_Thread* CPU{};
	}

	void init();
	int cycle();
	void quit();
}

#endif