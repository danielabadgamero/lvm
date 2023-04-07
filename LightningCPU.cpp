#include "LightningCore.h"
#include "LightningCPU.h"

#define dest getDest()
#define source getSource()

static void readMemory(short address, unsigned char* data)
{
	*data = Lightning::Core::RAM[address];
}

static void writeMemory(short address, unsigned char data)
{
	Lightning::Core::RAM[address] = data;
}

void Lightning::CPU::decode()
{
	switch (instruction.opcode)
	{
	}
}

int Lightning::CPU::cycle(void*)
{
	while (Core::running)
	{
		readMemory(regs[pc], (unsigned char*)&instruction);
		if (instruction.aMode == 0)
			regs[pc] += 2;
		else
			regs[pc] += 4;
		decode();
	}

	return 0;
}