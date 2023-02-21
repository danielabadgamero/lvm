#include "LightningCore.h"
#include "LightningCPU.h"

void Lightning::CPU::decode()
{
	switch (ir.opcode)
	{
	case HALT:
		running = false;
		break;
	case MOVI:
		if (ir.addrMode == 0)
			reg[ir.reg] = ir.op2;
		break;
	case MOVR:
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (!running);

	while (running)
	{
		systemBus.address = pc;
		systemBus.control.read = 1;
		while (systemBus.control.read);
		ir.instruction = systemBus.data;
		pc++;
		decode();
	}

	return 0;
}