#ifndef LIGHTNING_MONITOR
#define LIGHTNING_MONITOR

namespace Lightning::Monitor
{
	inline SDL_Texture* screen{};
	inline Uint8* pixels{};

	inline int pixelSize{};
	inline int pitch{};

	void refresh();
}

#endif