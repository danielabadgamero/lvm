#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ir.bitfields.opcode
#define rDest reg[ir.bitfields.reg]
#define rSource reg[ir.bitfields.op2]
#define imm ir.bitfields.op2
#define aMode ir.bitfields.addrMode

static void readMemory(int address, int* dest)
{
	if (Lightning::Core::chipSelected)
		*dest = Lightning::Core::RAM[address];
	else
		*dest = Lightning::Core::ROM[address];
}

static void writeMemory(int address, int data)
{
	Lightning::Core::RAM[address] = (unsigned char)data;
}

void Lightning::CPU::decode()
{
	switch (opcode)
	{
	case MOV:
		if (aMode == 0)
			rDest = rSource;
		else
			rDest = imm;
		break;
	case LD:
		if (aMode == 0)
			readMemory(rSource, &rDest);
		else
			readMemory(imm, &rDest);
		break;
	case ST:
		if (aMode == 0)
			writeMemory(rDest, rSource);
		else
			writeMemory(rDest, imm);
		break;
	case PUSH:
		if (aMode == 0)
			stack.push(rSource);
		else
			stack.push(imm);
		break;
	case POP:
		rDest = stack.top();
		stack.pop();
		break;
	case HALT:
		if (aMode == 0)
			Core::running = false;
		else
			Core::chipSelected = imm;
		break;
	case CMP:
		if (aMode == 0)
		{
			compFlags[equal] = rDest == rSource;
			compFlags[greater] = rDest > rSource;
			compFlags[greater_equal] = rDest >= rSource;
			compFlags[less] = rDest < rSource;
			compFlags[less_equal] = rDest <= rSource;
		}
		else
		{
			compFlags[equal] = rDest == imm;
			compFlags[greater] = rDest > imm;
			compFlags[greater_equal] = rDest >= imm;
			compFlags[less] = rDest < imm;
			compFlags[less_equal] = rDest <= imm;
		}
		break;
	case JMP:
		if (aMode == 0)
			pc = rSource;
		else
			pc = imm;
		break;
	case JEQ:
		if (compFlags[equal])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case ADD:
		if (aMode == 0)
			rDest += rSource;
		else
			rDest += imm;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
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