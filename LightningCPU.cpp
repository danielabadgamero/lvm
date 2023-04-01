#include "LightningCore.h"
#include "LightningCPU.h"

#define dest getDest()
#define source getSource()

static unsigned char readMemory(int address)
{
	if (Lightning::Core::chipSelected)
		return Lightning::Core::RAM[address];
	else
		return Lightning::Core::ROM[address];
}

static void writeMemory(int address, unsigned char data)
{
	Lightning::Core::RAM[address] = data;
}

static int& getDest()
{
	if (Lightning::CPU::instruction.dAddr == 0)
		return Lightning::CPU::stack.top();
	else
		return Lightning::CPU::regs[Lightning::CPU::instruction.dAddr - 1];
}

static int getSource()
{
	Lightning::CPU::regs[Lightning::CPU::pc]++;
	if (Lightning::CPU::instruction.aMode == 0)
		return readMemory(Lightning::CPU::regs[Lightning::CPU::pc] - 1);
	else
	{
		int imm{};
		imm = readMemory(Lightning::CPU::regs[Lightning::CPU::pc] - 1);
		imm <<= 8;
		imm += readMemory(Lightning::CPU::regs[Lightning::CPU::pc]);
		imm <<= 8;
		Lightning::CPU::regs[Lightning::CPU::pc]++;
		imm += readMemory(Lightning::CPU::regs[Lightning::CPU::pc]);
		Lightning::CPU::regs[Lightning::CPU::pc]++;
		return imm;
	}
}

void Lightning::CPU::decode()
{
	switch (instruction.opcode)
	{
	case HALT:
		Core::chipSelected = instruction.dAddr;
		break;
	case MOV:
		dest = source;
		break;
	case MGT:
		if (Core::flags[Core::greater])
			dest = source;
		break;
	case MLT:
		if (Core::flags[Core::less])
			dest = source;
		break;
	case MEQ:
		if (Core::flags[Core::equal])
			dest = source;
		break;
	case POP:
		dest = stack.top();
		stack.pop();
		break;
	case RD:
		dest = readMemory(source);
		break;
	case WR:
		writeMemory(dest, static_cast<unsigned char>(source));
		break;
	case RDSK:
		SDL_memcpy(&Core::RAM[dest], Core::disk[source], 512);
		break;
	case WDSK:
		SDL_memcpy(Core::disk[dest], &Core::RAM[source], 512);
		break;
	case ADD:
		dest += source;
		break;
	case MUL:
		dest *= source;
		break;
	case DIV:
		dest /= source;
		break;
	case CMP:
		Core::flags[Core::greater] = dest > source;
		Core::flags[Core::less] = dest < source;
		Core::flags[Core::equal] = dest == source;
		break;
	case AND:
		dest &= source;
		break;
	case NOT:
		dest = ~dest;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (!Core::running);

	while (Core::running)
	{
		instruction = readMemory(regs[pc]);
		regs[pc]++;
		decode();
	}

	return 0;
}