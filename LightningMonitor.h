#ifndef LIGHTNING_MONITOR
#define LIGHTNING_MONITOR

#include <SDL.h>

#include "LightningCore.h"

namespace Lightning::Monitor
{
	inline SDL_Texture* screen{};
	inline Uint16* pixels{};

	inline int* rSize{ &peripherals[monitor][0] };
	inline int* rPitch{ &peripherals[monitor][1] };

	void refresh();
}

#endif