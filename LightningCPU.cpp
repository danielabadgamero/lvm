#include "LightningCore.h"
#include "LightningCPU.h"

#define dest (instruction.dAddr == 0) ? 

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

void Lightning::CPU::decode()
{
	switch (instruction.opcode)
	{

	}
}

int Lightning::CPU::cycle(void*)
{
	while (!Core::running);

	while (Core::running)
	{
		instruction = readMemory(regs[pc]);
		decode();
	}

	return 0;
}