#include "LightningCore.h"
#include "LightningCPU.h"

int CPU::cycle(void* data)
{
	char* RAM{ (char*)data };
	RAM[Lightning::VIDEO_TXT] = 'H';
	RAM[Lightning::VIDEO_TXT + 1] = 'e';
	RAM[Lightning::VIDEO_TXT + 2] = 'l';
	RAM[Lightning::VIDEO_TXT + 3] = 'l';
	RAM[Lightning::VIDEO_TXT + 4] = 'o';
	RAM[Lightning::VIDEO_TXT + 5] = ',';
	RAM[Lightning::VIDEO_TXT + 6] = ' ';
	RAM[Lightning::VIDEO_TXT + 7] = 'w';
	RAM[Lightning::VIDEO_TXT + 8] = 'o';
	RAM[Lightning::VIDEO_TXT + 9] = 'r';
	RAM[Lightning::VIDEO_TXT + 10] = 'l';
	RAM[Lightning::VIDEO_TXT + 11] = 'd';
	RAM[Lightning::VIDEO_TXT + 12] = '!';

	while (Lightning::running)
	{
	}

	return 0;
}