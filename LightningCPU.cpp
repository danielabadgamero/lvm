#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	while (!running);

	RAM[VIDEO] = 0b10000000'00000000'00000000'00000000;

	while (running)
	{
		switch (RAM[PC])
		{
		case 0:
			break;
		}
	}

	return 0;
}