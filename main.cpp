#include <iostream>

#include "LightningCore.h"

int main()
{
	Lightning::init();

	while (Lightning::running)
	{
		Lightning::CPU.REG[IR] = Lightning::RAM[Lightning::CPU.PC];
		Lightning::CPU.REG[IR] <<= 8;
		Lightning::CPU.REG[IR] += Lightning::RAM[Lightning::CPU.PC + 1];
		Lightning::CPU.REG[IR] <<= 8;
		Lightning::CPU.REG[IR] += Lightning::RAM[Lightning::CPU.PC + 2];
		Lightning::CPU.REG[IR] <<= 8;
		Lightning::CPU.REG[IR] += Lightning::RAM[Lightning::CPU.PC + 3];
		Lightning::CPU.process();
		Lightning::CPU.PC += 4;
	}

	return 0;
}