#include "LightningCore.h"
#include "LightningCPU.h"

#define op ((Lightning::CPU::reg[Lightning::CPU::ir] & Format::opcode) >> 27)
#define r1 Lightning::CPU::reg[((Lightning::CPU::reg[Lightning::CPU::ir] & Format::reg1) >> 24)]
#define r2 Lightning::CPU::reg[((Lightning::CPU::reg[Lightning::CPU::ir] & Format::reg2) >> 21)]
#define cond ((Lightning::CPU::reg[Lightning::CPU::ir] & Format::cnd) >> 18)
#define imm21 (Lightning::CPU::reg[Lightning::CPU::ir] & Format::im21)
#define imm24 (Lightning::CPU::reg[Lightning::CPU::ir] & Format::im24)
#define stack Lightning::CPU::stack
#define sb Lightning::CPU::sb
#define bistable(n) Lightning::CPU::bistables[Lightning::CPU::n]

namespace Format
{
	constexpr static unsigned int opcode{ 0b11111000'00000000'00000000'00000000 };
	constexpr static unsigned int reg1	{ 0b00000111'00000000'00000000'00000000 };
	constexpr static unsigned int reg2	{ 0b00000000'11100000'00000000'00000000 };
	constexpr static unsigned int cnd	{ 0b00000000'00011100'00000000'00000000 };
	constexpr static unsigned int im21	{ 0b00000000'00011111'11111111'11111111 };
	constexpr static unsigned int im24	{ 0b00000000'11111111'11111111'11111111 };
}

static void decode()
{
	switch (op)
	{
	case 0x00:	// HALT
		bistable(running).flip();
		break;
	case 0x01:	// MOVE reg - reg
		r1 = r2;
		break;
	case 0x02:	// MOVE reg - mem
		r1 = Lightning::RAM[r2];
		break;
	case 0x03:	// MOVE mem - reg
		Lightning::RAM[r1] = r2;
		break;
	case 0x04:	// MOVE reg - imm24
		r1 = imm24;
		break;
	case 0x05:	// MOVE mem - imm24
		Lightning::RAM[r1] = imm24;
		break;
	case 0x06:	// MOVE reg - RAM[imm24]
		r1 = Lightning::RAM[imm24];
		break;
	case 0x07:	// MOVE RAM[imm24] - reg
		Lightning::RAM[imm24] = r1;
		break;
	case 0x08:	// ADD reg - reg
		r1 += r2;
		break;
	case 0x09:	// ADD reg - imm24
		r1 += imm24;
		break;
	case 0x0A:	// SUB reg - reg
		r1 -= r2;
		break;
	case 0x0B:	// SUB reg - imm24
		r1 -= imm24;
		break;
	case 0x0C:	// MUL reg - reg
		r1 *= r2;
		break;
	case 0x0D:	// MUL reg - imm24
		r1 *= imm24;
		break;
	case 0x0E:	// DIV reg - reg
		r1 /= r2;
		break;
	case 0x0F:	// DIV reg - imm24
		r1 /= imm24;
		break;
	case 0x10:	// IN reg - addr - reg
		r1 = Lightning::CPU::peripherals[r2][cond];
		break;
	case 0x11:	// OUT reg - addr - reg
		Lightning::CPU::peripherals[r2][cond] = r1;
		break;
	case 0x12:
		break;
	case 0x13:
		break;
	case 0x14:
		break;
	case 0x15:
		break;
	case 0x16:
		break;
	case 0x17:
		break;
	case 0x18:
		break;
	case 0x19:
		break;
	case 0x1A:
		break;
	case 0x1B:
		break;
	case 0x1C:
		break;
	case 0x1D:
		break;
	case 0x1E:
		break;
	case 0x1F:
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (~bistables[running])
	{
		reg[ir] = (ROM[reg[pc]] << 24) + (ROM[reg[pc] + 1] << 16) + (ROM[reg[pc] + 2] << 8) + ROM[reg[pc] + 3];
		reg[pc] += 4;
		decode();
	}

	reg[pc] = 0;

	while (bistables[running])
	{
		reg[ir] = (RAM[reg[pc]] << 24) + (RAM[reg[pc] + 1] << 16) + (RAM[reg[pc] + 2] << 8) + RAM[reg[pc] + 3];
		reg[pc] += 4;
		decode();
	}

	return 0;
}