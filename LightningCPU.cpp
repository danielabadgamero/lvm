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
	if (Lightning::CPU::instruction.aMode == 0)
		return Lightning::CPU::regs[readMemory(Lightning::CPU::regs[Lightning::CPU::pc] - 1) - 1];
	else
	{
		int imm{};
		imm = readMemory(Lightning::CPU::regs[Lightning::CPU::pc] - 3);
		imm <<= 8;
		imm += readMemory(Lightning::CPU::regs[Lightning::CPU::pc] - 2);
		imm <<= 8;
		imm += readMemory(Lightning::CPU::regs[Lightning::CPU::pc] - 1);
		return imm;
	}
}

void Lightning::CPU::decode()
{
	switch (instruction.opcode)
	{
	case SWTCH:
		Core::chipSelected = instruction.dAddr;
		break;
	case MOV:
 		if (instruction.dAddr == 0)
			stack.push(0);
		dest = source;
		if (instruction.dAddr == pc + 1 || instruction.dAddr == tr + 1)
			dest += regs[pb];
		break;
	case MGT:
		if (Core::flags[Core::greater])
		{
			if (instruction.dAddr == 0)
				stack.push(0);
			dest = source;
			if (instruction.dAddr == pc + 1 || instruction.dAddr == tr + 1)
				dest += regs[pb];
		}
		break;
	case MEQ:
		if (Core::flags[Core::equal])
		{
			if (instruction.dAddr == 0)
				stack.push(0);
			dest = source;
			if (instruction.dAddr == pc + 1 || instruction.dAddr == tr + 1)
				dest += regs[pb];
		}
		break;
	case POP:
		dest = stack.top();
		stack.pop();
		break;
	case RD:
		if (instruction.dAddr == 0)
			stack.push(0);
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
	case SUB:
		dest -= source;
		break;
	case MUL:
		dest *= source;
		break;
	case DIV:
		dest /= source;
		break;
	case CMP:
		Core::flags[Core::greater] = dest > source;
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
	while (Core::running)
	{
		instruction = readMemory(regs[pc]);
		if (instruction.aMode == 0)
			regs[pc] += 2;
		else
			regs[pc] += 4;
		decode();
	}

	return 0;
}