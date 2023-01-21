#ifndef LIGHTNING_MONITOR
#define LIGHTNING_MONITOR

#include <SDL.h>

#include "LightningCPU.h"

namespace Lightning::Monitor
{
	inline SDL_Texture* screen{};
	inline Uint8* pixels{};

	inline int pixelSize{};
	inline int pitch{};

	inline int* rAddr{ &CPU::peripherals[CPU::monitor][0] };
	inline int* rData{ &CPU::peripherals[CPU::monitor][1] };

	inline unsigned char VRAM[1 << 24]{};

	void refresh();
}

#endif