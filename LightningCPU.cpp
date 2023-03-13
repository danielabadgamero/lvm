#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ir.bitfields.opcode
#define rDest reg[ir.bitfields.reg]
#define rSource reg[ir.bitfields.op2]
#define imm static_cast<int>((ir.bitfields.op2 & 0x800000 ? (0xff000000 | ir.bitfields.op2) : ir.bitfields.op2))
#define aMode ir.bitfields.addrMode
#define value ((aMode == 0) ? rSource : imm)

static void readMemory(int address, int* dest)
{
	if (Lightning::Core::chipSelected)
		*dest = Lightning::Core::RAM[address];
	else
		*dest = Lightning::Core::ROM[address];
}

static void writeMemory(int address, int data)
{
	Lightning::Core::RAM[address] = static_cast<unsigned char>(data);
}

void Lightning::CPU::decode()
{
	switch (opcode)
	{
		//	Data management
	case MOV:
		rDest = value;
		break;
	case UNUSED_1:
		break;
	case LD:
		readMemory(value, &rDest);
		break;
	case ST:
		writeMemory(rDest, value);
		break;
	case PUSH:
		stack.push(value);
		break;
	case POP:
		rDest = stack.top();
		stack.pop();
		break;
	case IN:
		break;
	case OUT:
		break;
		
		//	Flow control
	case HALT:
		if (aMode == 0)
			Core::running = false;
		else
			Core::chipSelected = imm;
		pc = 0;
		break;
	case CMP:
		compFlags[equal] = rDest == value;
		compFlags[greater] = rDest > value;
		compFlags[greater_equal] = rDest >= value;
		compFlags[less] = rDest < value;
		compFlags[less_equal] = rDest <= value;
		break;
	case JMP:
		pc = value;
		break;
	case JEQ:
		if (compFlags[equal])
			pc = value;
		break;
	case JNE:
		if (compFlags[not_equal])
			pc = value;
		break;
	case JGT:
		if (compFlags[greater])
			pc = value;
		break;
	case JGE:
		if (compFlags[greater_equal])
			pc = value;
		break;
	case JLT:
		if (compFlags[less])
			pc = value;
		break;
	case JLE:
		if (compFlags[less_equal])
			pc = value;
		break;
	case CALL:
		stack.push(pc);
		pc = value;
		break;
	case RET:
		pc = stack.top();
		stack.pop();
		break;
	case INT:
		interruptTable[imm]();
		break;

		//	Arithmetic
	case ADD:
		rDest += value;
		break;
	case SUB:
		rDest -= value;
		break;
	case MUL:
		rDest *= value;
		break;
	case DIV:
		rDest /= value;
		break;
	case MOD:
		rDest %= value;
		break;
	case SHFT:
		rDest <<= value;
		break;

		//	Logic
	case AND:
		rDest &= value;
		break;
	case NAND:
		rDest = ~(value & rDest);
		break;
	case OR:
		rDest |= value;
		break;
	case XOR:
		rDest ^= value;
		break;
	case NOR:
		rDest = ~(value | rDest);
		break;
	case NOT:
		rDest = ~value;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	interruptTable[print_char] = []()
	{
		char c{ static_cast<char>(reg[ax]) };

		for (int i{}; i != 32; i++)
			for (int j{}; j != 3; j++)
				for (int k{}; k != 8; k++)
				{
					Core::RAM[VIDEO + (cursor.y + i) * pixelsPitch + cursor.x * 3 + j * 24 + k * 3] ^= ((Core::font[c * 96 + i * 3 + j] & (1 << (7 - k))) >> (7 - k)) * 0xff;
					Core::RAM[VIDEO + (cursor.y + i) * pixelsPitch + cursor.x * 3 + j * 24 + k * 3 + 1] ^= ((Core::font[c * 96 + i * 3 + j] & (1 << (7 - k))) >> (7 - k)) * 0xff;
					Core::RAM[VIDEO + (cursor.y + i) * pixelsPitch + cursor.x * 3 + j * 24 + k * 3 + 2] ^= ((Core::font[c * 96 + i * 3 + j] & (1 << (7 - k))) >> (7 - k)) * 0xff;
				}

		if (cursor.x >= 1896)
			interruptTable[new_line]();
		else
			cursor.x += 24;
	};

	interruptTable[new_line] = []()
	{
		if (cursor.y >= 1024)
			for (int i{ VIDEO }; i != VIDEO + pixelsSize; i++)
				Core::RAM[i] = Core::RAM[i + 32 * pixelsPitch];
		else
			cursor.y += 32;
		cursor.x = 0;
	};

	interruptTable[read_disk] = []()
	{
		for (int i{}; i != reg[bx]; i++)
			SDL_memcpy(&Core::RAM[reg[cx] + i * 512], Core::disk[reg[ax] + i], 512);
	};

	interruptTable[write_disk] = []()
	{
		for (int i{}; i != reg[bx]; i++)
			SDL_memcpy(Core::disk[reg[cx] + i], &Core::RAM[reg[ax] + i * 512], 512);
	};

	while (!Core::running);

	while (Core::running)
	{
		int instruction{};
		for (int i{}; i != 4; i++)
		{
			readMemory(pc, &instruction);
			ir.instruction <<= 8;
			ir.instruction += instruction;
			pc++;
		}
		decode();
	}

	return 0;
}