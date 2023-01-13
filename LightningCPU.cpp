#include "LightningCore.h"
#include "LightningCPU.h"

int CPU::cycle(void* data)
{
	char* RAM{ (char*)data };

	while (Lightning::running)
	{
		RAM[Lightning::VIDEO_TXT] = RAM[Lightning::KEY_PRESS];
	}

	return 0;
}