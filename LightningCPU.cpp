#include "LightningCore.h"
#include "LightningCPU.h"

#define op ((Lightning::CPU::reg[Lightning::CPU::ir] & Format::opcode) >> 27)
#define r1 Lightning::CPU::reg[((Lightning::CPU::reg[Lightning::CPU::ir] & Format::reg1) >> 24)]
#define r2 Lightning::CPU::reg[((Lightning::CPU::reg[Lightning::CPU::ir] & Format::reg2) >> 21)]
#define cond ((Lightning::CPU::reg[Lightning::CPU::ir] & Format::cnd) >> 18)
#define im21 (Lightning::CPU::reg[Lightning::CPU::ir] & Format::imm21)
#define im24 (Lightning::CPU::reg[Lightning::CPU::ir] & Format::imm24)
#define stack Lightning::CPU::stack
#define sb Lightning::CPU::sb
#define bistable Lightning::CPU::bistables

namespace Format
{
	constexpr static unsigned int opcode{ 0b11111000'00000000'00000000'00000000 };
	constexpr static unsigned int reg1	{ 0b00000111'00000000'00000000'00000000 };
	constexpr static unsigned int reg2	{ 0b00000000'11100000'00000000'00000000 };
	constexpr static unsigned int cnd	{ 0b00000000'00011100'00000000'00000000 };
	constexpr static unsigned int imm21	{ 0b00000000'00011111'11111111'11111111 };
	constexpr static unsigned int imm24	{ 0b00000000'11111111'11111111'11111111 };
}

static void decode()
{
	switch (op)
	{
	case 0x00:	// HALT
		Lightning::CPU::bistables.running ^= 1;
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
	case 0x04:	// PUSH reg
		stack[sb] = r1;
		sb++;
		break;
	case 0x05:	// POP reg
		sb--;
		r1 = stack[sb];
		stack[sb] = 0;
		break;
	case 0x06:	// CMP reg - reg
		bistable.equal = (r1 == r2);
		bistable.not_equal = (r1 != r2);
		bistable.greater = (r1 > r2);
		bistable.greater_equal = (r1 >= r2);
		bistable.less = (r1 < r2);
		bistable.less_equal = (r1 <= r2);
		break;
	case 0x07:	// MOVE reg - reg - cond
		if (bistable.bistables & cond)
			r1 = r2;
		break;
	case 0x08:
		break;
	case 0x09:
		break;
	case 0x0A:
		break;
	case 0x0B:
		break;
	case 0x0C:
		break;
	case 0x0D:
		break;
	case 0x0E:
		break;
	case 0x0F:
		break;
	case 0x10:
		break;
	case 0x11:
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
	while (~bistables.running)
	{
		reg[ir] = (ROM[reg[pc]] << 24) + (ROM[reg[pc] + 1] << 16) + (ROM[reg[pc] + 2] << 8) + ROM[reg[pc] + 3];
		reg[pc] += 4;
		decode();
	}

	while (bistables.running)
	{
		reg[ir] = (RAM[reg[pc]] << 24) + (RAM[reg[pc] + 1] << 16) + (RAM[reg[pc] + 2] << 8) + RAM[reg[pc] + 3];
		reg[pc] += 4;
		decode();
	}

	return 0;
}