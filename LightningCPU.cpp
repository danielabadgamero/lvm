#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ((instruction & 0xf000) >> 12)
#define rDest (reg[(instruction & 0x0e00) >> 9])
#define source8 static_cast<char>(((instruction & 0x0100) == 0) ? reg[instruction & 0x00ff] : (instruction & 0x00ff))
#define source5 (((instruction & 0x0100) == 0) ? reg[(instruction & 0x00f8) >> 3] : ((instruction & 0x00f8) >> 3))
#define cond ((condReg & 0x0007) == 1)

void Lightning::CPU::decode()
{
	switch (opcode)
	{
	case HLT:
		running = false;
		break;
	case MOV:
		rDest = source8;
		break;
	case ADD:
		rDest += source8;
		break;
	case SUB:
		rDest -= source8;
		break;
	case MUL:
		rDest *= source8;
		break;
	case DIV:
		rDest /= source8;
		break;
	case AND:
		rDest &= source8;
		break;
	case OR:
		rDest |= source8;
		break;
	case SHIFT:
		rDest >>= source8;
		break;
	case PUSH:
		stack.push(source8);
		break;
	case POP:
		stack.pop();
		break;
	case CALL:
		stack.push(reg[pc]);
		reg[pc] = source8;
		break;
	case RET:
		reg[pc] = stack.top();
		stack.pop();
		break;
	case MOVC:
		if (cond)
			rDest = source5;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (running)
	{
		instruction = RAM[reg[pc]];
		reg[pc]++;
		decode();
	}

	return 0;
}