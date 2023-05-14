#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <vector>
#include <string>
#include <bitset>
#include <stack>

#include <SDL.h>
#include <SDL_ttf.h>

namespace Lightning
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_DisplayMode screen{};
	inline TTF_Font* font{};
	inline SDL_Thread* thread{};
	inline bool running{};

	inline std::vector<std::string> opcodes
	{
		// Data management
		"MOV",
		"PUSH",
		"POP",
		"RD",
		"WR",
		"REC",

		// Flow control
		"HALT",
		"JMP",
		"CALL",
		"RET",

		// Arithmetic and bitwise logic
		"ADD",
		"MUL",
		"DIV",
		"CMP",
		"AND",
		"NOT",
	};

	inline std::vector<std::string> flags
	{
		"TRUE",
		"EQUAL",
		"NOT_EQUAL",
		"LESS",
		"GREATER",
		"ZERO",
		"NOT_ZERO",
		"CARRY",
	};

	inline std::vector<std::string> regs
	{
		"a",
		"b",
		"c",
		"d",
	};

	inline std::stack<long long> stack{};
	inline std::vector<long long> sysFuncs{};
	inline std::bitset<8> flag{};
	inline short pc{};
	inline short pb{};
	inline long long reg[4]{};
	inline char RAM[1 << 16]{};
	inline char disk[1 << 16][512]{};

	inline struct Op
	{
		unsigned char opcode : 4;
		unsigned char dest : 1;
		unsigned char ddMode : 1;
		unsigned char src : 1;
		unsigned char sdMode : 1;

		unsigned char sReg{};
		short sImm{};
		unsigned char dReg{};
		short dImm{};

		void operator=(char);
	} op{};
	
	void reset();
	void loop();
	int loop(void*);
	void quit();
}

#endif