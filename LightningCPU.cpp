#include "LightningCore.h"
#include "LightningCPU.h"

void Lightning::CPU::decode()
{
	switch (ir.opcode)
	{
	case HALT:
		if (Core::systemBus.control.chipSelect == 0)
			Core::systemBus.control.chipSelect = 1;
		else
			Core::running = false;
		break;
	case MOVI:
		if (ir.addrMode == 0)
			reg[ir.reg] = ir.op2;
		else
		{
			Core::systemBus.address = ir.op2;
			Core::systemBus.control.read = 1;
			while (Core::systemBus.control.read);
			reg[ir.reg] = Core::systemBus.data;
		}
		break;
	case MOVR:
		if (ir.addrMode == 0)
			reg[ir.reg] = reg[ir.op2];
		else
		{
			Core::systemBus.address = reg[ir.op2];
			Core::systemBus.control.read = 1;
			while (Core::systemBus.control.read);
			reg[ir.reg] = Core::systemBus.data;
		}
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (!Core::running);

	while (Core::running)
	{
		Core::systemBus.address = pc;
		Core::systemBus.control.read = 1;
		while (Core::systemBus.control.read);
		ir.instruction = Core::systemBus.data;
		pc++;
		decode();
	}

	return 0;
}