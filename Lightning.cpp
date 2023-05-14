#include <stack>
#include <bitset>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Lightning.h"
#include "Core.h"

struct Bytes
{
	int start{};
	int size{};
};

void Lightning::Op::operator=(char byte)
{
	opcode = (byte & 0xf0) >> 4;
	dest = (byte & 0x08) >> 3;
	ddMode = (byte & 0x04) >> 2;
	src = (byte & 0x02) >> 1;
	sdMode = byte & 0x01;
}

static Bytes getBytes(unsigned char reg)
{
	Bytes bytes{ 0, 8 };

	int selBytes{ (reg & 0x30) >> 4 };
	if (selBytes == 0b11) return bytes;
	bytes.size = 4;
	if (selBytes == 0b01)
		bytes.start += 4;

	selBytes = (reg & 0x0c) >> 2;
	if (selBytes == 0b11) return bytes;
	bytes.size = 2;
	if (selBytes == 0b01)
		bytes.start += 2;

	selBytes = (reg & 0x03);
	if (selBytes == 0b11) return bytes;
	bytes.size = 1;
	if (selBytes == 0b01)
		bytes.start += 1;

	return bytes;
}

void Lightning::reset()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	memset(RAM, 0, 1ll << 16);
	memset(reg, 0, 4 * sizeof(long long));
	pc = pb = 0;
	flag.reset();
	flag.set(0);
	while (!stack.empty()) stack.pop();

	SDL_GetDesktopDisplayMode(0, &screen);
	window = SDL_CreateWindow("LightningVM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.w, screen.h, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_ShowCursor(SDL_DISABLE);
	font = TTF_OpenFont("C:\\Windows\\Fonts\\courbd.ttf", 32);

	running = true;
	thread = SDL_CreateThread(loop, "CPU", NULL);
}

long long getRegVal(unsigned char regByte)
{
	long long src{};

	Bytes bytes{ getBytes(regByte) };
	unsigned char* dBytes{ (unsigned char*)&src + (7ull - bytes.start) };
	for (int b{ bytes.size - 1 }; b >= 0; b--)
		*(dBytes - b) = (Lightning::reg[(regByte & 0xc0) >> 6] & (0xffull << (bytes.size - b - 1) * 8)) >> (bytes.size - b - 1) * 8;

	return src;
}

void Lightning::loop()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e))
		switch (e.type)
		{
		case SDL_KEYDOWN:
			break;
		}
	
	SDL_RenderClear(renderer);

	if (running)
		for (int i{}; i != screen.h / 32; i++)
			for (int j{}; j != screen.w / 20; j++)
			{
				SDL_Surface* glyph{ TTF_RenderGlyph_Solid(font, RAM[0xf000 + i * (screen.h / 32) + j], { 0xff, 0xff, 0xff }) };
				SDL_Texture* texture{ SDL_CreateTextureFromSurface(renderer, glyph) };
				SDL_Rect rect{ j * 20, i * 32 };
				SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
				SDL_RenderCopy(renderer, texture, NULL, &rect);
				SDL_FreeSurface(glyph);
				SDL_DestroyTexture(texture);
			}
	else quit();

	SDL_RenderPresent(renderer);
}

int Lightning::loop(void*)
{
	while (running)
	{
		op = RAM[pc];
		pc++;
		if (op.dest == 0) (op.dReg = RAM[pc]), pc++;
		else (op.dImm = (RAM[pc] << 8) | RAM[pc + 1]), pc += 2;
		if (op.src == 0) (op.sReg = RAM[pc]), pc++;
		else (op.sImm = (RAM[pc] << 8) | RAM[pc + 1]), pc += 2;

		long long src{};
		long long dest{};

		if (op.src == 0)
			if (op.sdMode == 0) src = getRegVal(op.sReg);
			else src = RAM[getRegVal(op.sReg)];
		else
			if (op.sdMode == 0) src = op.sImm;
			else src = RAM[op.sImm];

		if (op.dest == 0)
			if (op.ddMode == 0) dest = getRegVal(op.dReg);
			else dest = RAM[getRegVal(op.dReg)];
		else
			if (op.ddMode == 0) dest = RAM[op.dImm];
			else dest = RAM[RAM[op.dImm]];

		bool write{ true };
		bool toDisk{ false };
		bool fromDisk{ false };

		switch (op.opcode)
		{
		case 0x0:
			dest = src;
			break;
		case 0x1:
			stack.push(src);
			write = false;
			break;
		case 0x2:
			dest = stack.top();
			stack.pop();
			break;
		case 0x3:
			fromDisk = true;
			break;
		case 0x4:
			toDisk = true;
			break;
		case 0x5:
			write = false;
			sysFuncs.push_back(src);
			break;
		case 0x6:
			running = false;
			return 0;
		case 0x7:
			write = false;
			if (flag.test(op.dReg)) { pc = src; break; }
			break;
		case 0x8:
			write = false;
			stack.push(pc);
			if (op.ddMode == 0) pc = src;
			else pc = sysFuncs[src];
			pb = pc;
			break;
		case 0x9:
			write = false;
			pc = stack.top();
			stack.pop();
			if (!stack.empty()) pb = stack.top();
			else pb = 0;
			break;
		case 0xa:
			dest += src;
			flag.set(5, dest == 0);
			flag.set(6, dest != 0);
			flag.set(7, dest > 0xff);
			break;
		case 0xb:
			dest *= src;
			flag.set(5, dest == 0);
			flag.set(6, dest != 0);
			flag.set(7, dest > 0xff);
			break;
		case 0xc:
			dest /= src;
			flag.set(5, dest == 0);
			flag.set(6, dest != 0);
			flag.set(7, dest > 0xff);
			break;
		case 0xd:
			write = false;
			flag.set(0);
			flag.set(1, dest == src);
			flag.set(2, dest != src);
			flag.set(3, dest < src);
			flag.set(4, dest > src);
			break;
		case 0xe:
			dest &= src;
			break;
		case 0xf:
			dest = ~src;
			break;
		}

		if (write)
		{
			if (op.dest == 0)
			{
				if (op.ddMode == 0)
				{
					Bytes bytes{ getBytes(op.dReg) };
					long long* dReg{ &reg[(op.dReg & 0xc0) >> 6] };
					unsigned char* dBytes{ (unsigned char*)dReg + (7ull - bytes.start) };
					for (int b{ bytes.size - 1 }; b >= 0; b--)
						*(dBytes - b) = (dest & (0xffull << (bytes.size - b - 1) * 8)) >> (bytes.size - b - 1) * 8;
				}
				else
				{
					Bytes bytes{ getBytes(op.dReg) };
					unsigned long long val{};
					long long rVal{ reg[(op.dReg & 0xc0) >> 6] };
					for (int b{}; b != bytes.size; b++)
						val |= rVal & (0xffull << (b + bytes.start));
					if (!toDisk && !fromDisk)
						RAM[val] = static_cast<char>(dest);
					else if (toDisk)
						memcpy_s(disk[val], 512, RAM + src, 512);
					else if (fromDisk)
						memcpy_s(RAM + val, 512, disk[src], 512);
				}
			}
			else
			{
				if (op.ddMode == 0)
				{
					if (!toDisk && !fromDisk)
						RAM[(uint16_t)op.dImm] = static_cast<char>(dest);
					else if (toDisk)
						memcpy_s(disk[(uint16_t)op.dImm], 512, RAM + (uint16_t)src, 512);
					else if (fromDisk)
						memcpy_s(RAM + (uint16_t)op.dImm, 512, disk[(uint16_t)src], 512);
				}
				else
				{
					if (!toDisk && !fromDisk)
						RAM[RAM[(uint16_t)op.dImm]] = static_cast<char>(dest);
					else if (toDisk)
						memcpy_s(disk[RAM[(uint16_t)op.dImm]], 512, RAM + (uint16_t)src, 512);
					else if (fromDisk)
						memcpy_s(RAM + RAM[(uint16_t)op.dImm], 512, disk[(uint16_t)src], 512);
				}
			}
		}
	}

	return -1;
}

void Lightning::quit()
{
	SDL_WaitThread(thread, NULL);
	TTF_CloseFont(font);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	TTF_Quit();
	SDL_Quit();

	Core::mode = Core::CMD;
}