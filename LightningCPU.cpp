#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	while (!running);

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