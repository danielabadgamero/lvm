#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

#include "LightningCPU.h"

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point windowSize{};
	inline bool running{ true };

	/*
	* Memory map:
	* 
	* 
	*/

	inline constexpr int VIDEO{ 0x100000 };

	inline short RAM[1 << 24]{};	// 16MB
	inline constexpr unsigned short ROM[1 << 12]	// 4KB
	{
		// read VIDEO number
		0b0001'101'1'00000100,	// set ar to 5
		0b0001'100'1'00000001,	// set cr to 1

		0b0001'000'0'00000110,	// move dr to r1
		0b1000'110'1'11110000,	// shift dr by -16

		0b0010'111'1'00000001,	// skip VIDEO data

		0b00000000'00010000,	// VIDEO

		0b0001'100'1'00000000,	// set cr to 0
		0b0001'101'0'00000000,	// set ar to r1
		0b0001'110'0'11111111,	// set dr to 0xff
		0b0001'100'1'00000011,	// set cr to 3

		0b0011'111'1'00000001,	// loop
	};


	enum Peripheral
	{
		monitor,
		disk,

		total_peripherals
	};
	inline int peripherals[total_peripherals][8]{};

	void init();
	void cycle();
	void quit();
}

#endif