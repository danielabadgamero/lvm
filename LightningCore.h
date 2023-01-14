#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <SDL.h>

#include <vector>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Event e{};

	inline bool running{};

	inline char RAM[1 << 29]{};

	inline int64_t rax{};
	inline int64_t rbx{};
	inline int64_t rcx{};
	inline int64_t rdx{};
	inline int64_t rsi{};
	inline int64_t rdi{};
	inline int64_t rbp{};
	inline int64_t rsp{};
	inline int64_t r8{};
	inline int64_t r9{};
	inline int64_t r10{};
	inline int64_t r11{};
	inline int64_t r12{};
	inline int64_t r13{};
	inline int64_t r14{};
	inline int64_t r15{};

	inline int32_t* eax{ (int32_t*)&rax };
	inline int32_t* ebx{ (int32_t*)&rbx };
	inline int32_t* ecx{ (int32_t*)&rcx };
	inline int32_t* edx{ (int32_t*)&rdx };
	inline int32_t* esi{ (int32_t*)&rsi };
	inline int32_t* edi{ (int32_t*)&rdi };
	inline int32_t* ebp{ (int32_t*)&rbp };
	inline int32_t* esp{ (int32_t*)&rsp };
	inline int32_t* r8d{ (int32_t*)&r8 };
	inline int32_t* r9d{ (int32_t*)&r9 };
	inline int32_t* r10d{ (int32_t*)&r10 };
	inline int32_t* r11d{ (int32_t*)&r11 };
	inline int32_t* r12d{ (int32_t*)&r12 };
	inline int32_t* r13d{ (int32_t*)&r13 };
	inline int32_t* r14d{ (int32_t*)&r14 };
	inline int32_t* r15d{ (int32_t*)&r15 };

	inline int16_t* ax{ (int16_t*)&rax };
	inline int16_t* bx{ (int16_t*)&rbx };
	inline int16_t* cx{ (int16_t*)&rcx };
	inline int16_t* dx{ (int16_t*)&rdx };
	inline int16_t* si{ (int16_t*)&rsi };
	inline int16_t* di{ (int16_t*)&rdi };
	inline int16_t* bp{ (int16_t*)&rbp };
	inline int16_t* sp{ (int16_t*)&rsp };
	inline int16_t* r8w{ (int16_t*)&r8 };
	inline int16_t* r9w{ (int16_t*)&r9 };
	inline int16_t* r10w{ (int16_t*)&r10 };
	inline int16_t* r11w{ (int16_t*)&r11 };
	inline int16_t* r12w{ (int16_t*)&r12 };
	inline int16_t* r13w{ (int16_t*)&r13 };
	inline int16_t* r14w{ (int16_t*)&r14 };
	inline int16_t* r15w{ (int16_t*)&r15 };

	inline int8_t* al{ (int8_t*)&rax };
	inline int8_t* bl{ (int8_t*)&rbx };
	inline int8_t* cl{ (int8_t*)&rcx };
	inline int8_t* dl{ (int8_t*)&rdx };
	inline int8_t* sil{ (int8_t*)&rsi };
	inline int8_t* dil{ (int8_t*)&rdi };
	inline int8_t* bpl{ (int8_t*)&rbp };
	inline int8_t* spl{ (int8_t*)&rsp };
	inline int8_t* r8b{ (int8_t*)&r8 };
	inline int8_t* r9b{ (int8_t*)&r9 };
	inline int8_t* r10b{ (int8_t*)&r10 };
	inline int8_t* r11b{ (int8_t*)&r11 };
	inline int8_t* r12b{ (int8_t*)&r12 };
	inline int8_t* r13b{ (int8_t*)&r13 };
	inline int8_t* r14b{ (int8_t*)&r14 };
	inline int8_t* r15b{ (int8_t*)&r15 };

	void init(SDL_Point);
	void cycle();
	void quit();
}

#endif