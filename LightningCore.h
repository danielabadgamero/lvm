#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

namespace Lightning
{
	inline constexpr int VIDEO_VGA{ 0x00000000 };

	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};
	inline SDL_Point* screen{};
	inline SDL_Point windowSize{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	inline int rax{};
	inline int rbx{};
	inline int rcx{};
	inline int rdx{};
	inline int rsi{};
	inline int rdi{};
	inline int rbp{};
	inline int rsp{};
	inline int r8{};
	inline int r9{};
	inline int r10{};
	inline int r11{};
	inline int r12{};
	inline int r13{};
	inline int r14{};
	inline int r15{};

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif