#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

namespace Lightning
{
	inline bool running{};

	inline char RAM[1 << 29]{};

	void init();
	void cycle();
	void quit();
}

#endif