#ifndef LIGHTNING_CPU
#define LIGHTNING_CPU

#include <SDL.h>

namespace Lightning::CPU
{
	inline SDL_Thread* thread;

	enum Opcode
	{
		HALT,	// HALT
		MOV,	// MOV	000 11111111
	};

	inline int pc{};

	inline int reg[4]{};

	inline int stack[1 << 16]{}; // 64 KB
	inline int sb{};

	struct
	{
		char running : 1{};
	} bistables{};

	int cycle(void*);
}

#endif