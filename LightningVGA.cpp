#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

int Lightning::VGA::cycle(void*)
{
	while (!running);

	while (running)
	{
		for (int i{}; i != WIDTH * HEIGHT; i++)
			if (RAM[VIDEO_VGA + i])
				screen[i] = { (i % WIDTH) * 5, (i / WIDTH) * 5, 5, 5 };
			else
				screen[i] = { -5, -5 };
	}

	return 0;
}