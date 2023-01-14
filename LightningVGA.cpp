#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

int Lightning::VGA::cycle(void*)
{
	while (!running);

	while (running)
	{
		for (int i{}; i != WIDTH * HEIGHT; i++)
			if (RAM[VIDEO + (i / 8)] & (1 << (i % 32)))
				screen[i] = { (i % WIDTH), (i / WIDTH) };
			else
				screen[i] = { -1, -1 };
	}

	return 0;
}