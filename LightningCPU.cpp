#include "LightningCore.h"
#include "LightningCPU.h"

void Lightning::CPU::decode()
{
}

int Lightning::CPU::cycle(void*)
{
	while (~(condReg & running))
	{
		instruction = (ROM[reg[pc]] << 8) + ROM[reg[pc] + 1];
		reg[pc] += 2;
		decode();
	}
	
	reg[pc] = 0;

	while (condReg & running)
	{
		instruction = (RAM[reg[pc]] << 8) + RAM[reg[pc] + 1];
		reg[pc] += 2;
		decode();
	}

	return 0;
}