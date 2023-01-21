#ifndef LIGHTNING_MONITOR
#define LIGHTNING_MONITOR

namespace Lightning::Monitor
{
	inline SDL_Texture* screen{};
	inline Uint8* pixels{};

	inline int pixelSize{};
	inline int pitch{};

	inline int rAddr{};
	inline int rData{};

	inline char VRAM[1 << 24]{};

	void refresh();
}

#endif