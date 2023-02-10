#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ((instruction & 0xf000) >> 12)
#define rDest (&REG[(instruction & 0x0e00) >> 9])
#define source8 (((instruction & 0x0100) == 0) ? REG[instruction & 0x00ff] : (instruction & 0x00ff))
#define source5 (((instruction & 0x0100) == 0) ? REG[(instruction & 0x00f8) >> 3] : ((instruction & 0x00f8) >> 3))
#define cond (condReg[instruction & 0x0007])

void Lightning::CPU::decode()
{
	switch (opcode)
	{
	case HLT:
		condReg ^= running;
		break;
	case MOV:
		break;
	case ADD:
		break;
	case SUB:
		break;
	case MUL:
		break;
	case DIV:
		break;
	case AND:
		break;
	case OR:
		break;
	case SHIFT:
		break;
	case LW:
		break;
	case SW:
		break;
	case PUSH:
		break;
	case POP:
		break;
	case CALL:
		break;
	case RET:
		break;
	case MOVC:
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (true)
	{
		instruction = (ROM[reg[pc]] << 8) + ROM[reg[pc] + 1];
		reg[pc] += 2;
		decode();
	}

	return 0;
}