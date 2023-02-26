#include "LightningCore.h"
#include "LightningCPU.h"

#include <iostream>

#define opcode ir.bitfields.opcode
#define rDest reg[ir.bitfields.reg]
#define rSource reg[ir.bitfields.op2]
#define imm ir.bitfields.op2
#define aMode ir.bitfields.addrMode

static void readMemory(int address, int* dest)
{
	Lightning::Core::systemBus.address = address;
	Lightning::Core::systemBus.control[Lightning::Core::read] = 1;
	while (Lightning::Core::systemBus.control[Lightning::Core::read]);
	*dest = Lightning::Core::systemBus.data;
}

static void writeMemory(int address, int data)
{
	Lightning::Core::systemBus.address = address;
	Lightning::Core::systemBus.data = data;
	Lightning::Core::systemBus.control[Lightning::Core::write] = 1;
	while (Lightning::Core::systemBus.control[Lightning::Core::write]);
}

void Lightning::CPU::decode()
{
	switch (opcode)
	{
	case HALT:
		if (aMode == 0)
			Core::running = false;
		else
			Core::systemBus.control[Core::chipSelect] = imm;
		break;
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
	}
}

int Lightning::CPU::cycle(void*)
{
	while (!Core::running);

	while (Core::running)
	{
		Core::systemBus.address = pc;
		Core::systemBus.control[Core::read] = 1;
		while (Core::systemBus.control[Core::read]);
		ir.instruction = Core::systemBus.data;
		pc++;
		decode();
	}

	return 0;
}