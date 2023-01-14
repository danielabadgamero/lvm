#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

#include "LightningCPU.h"

namespace Lightning
{
	constexpr inline unsigned int VIDEO_TXT{ 0x00001000 };
	constexpr inline unsigned int KEY_STATE{ 0x00200000 };
	constexpr inline unsigned int KEY_CHAR { 0x00200200 };

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_DisplayMode screen{};
	inline TTF_Font* font{};
	inline std::vector<SDL_Texture*> glyphs{};

	inline bool running{};

	inline char RAM[1 << 29]
	{
		CPU::SAR, static_cast<char>(VIDEO_TXT >> 16), static_cast<char>(VIDEO_TXT >> 8), static_cast<char>(VIDEO_TXT),

		CPU::PUSH, 0, CPU::AR, 0,
		CPU::SAR, static_cast<char>(KEY_CHAR >> 16), static_cast<char>(KEY_CHAR >> 8), static_cast<char>(KEY_CHAR),
		CPU::RMEM, 0, 0, 0,
		CPU::SEQ, CPU::TR, CPU::DR, 0,
		CPU::JNZI, 0, CPU::TR, -2,
		CPU::POP, CPU::AR, 0, 0,
		CPU::WMEM, 0, 0, 0,
		CPU::INC, CPU::AR, 0, 2,
		CPU::SDR, 0, 0, 127,
		CPU::WMEM, 0, 0, 0,
		CPU::INC, CPU::AR, 0, -1,
		CPU::JPI, 0, 0, -11,
	};

	void init();
	void cycle();
	void quit();
}

#endif