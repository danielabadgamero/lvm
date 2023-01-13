#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	RAM[VIDEO_TXT] = 'a';

	while (Lightning::running)
	{
	}

	return 0;
}