#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>
#include <SDL_ttf.h>

inline SDL_Window* window{};
inline SDL_Renderer* renderer{};
inline SDL_Event e{};
inline SDL_DisplayMode screen{};
inline SDL_Texture* monitor{};
inline Uint8* pixels{};
inline TTF_Font* font{};

inline int videoSize{};
inline int videoPitch{};

inline constexpr int VIDEO{ 0xf50000 };

namespace Lightning::Core
{
	inline bool running{};

	inline unsigned char RAM[1 << 24]{};
	inline constexpr unsigned char ROM[]
	{
		0b0001'000'1, 0x00, 0x00, 0x0c,	// destination address
		0b0111'000'1, 0x00, 0x00, 0x00,	// source sector
		0b0100'000'0, 0x00,				// clean stack
		0b0000'001'0, 0x00,				// switch to RAM
	};

	inline SDL_Thread* CPU_Thread{};

	void init();
	void cycle();
	void quit();
}

#endif