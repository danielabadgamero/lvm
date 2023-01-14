#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

int Lightning::VGA::cycle(void*)
{
	while (!running);

	while (running)
	{
		for (int i{}; i != windowSize.x * windowSize.y; i++)
			if (RAM[VIDEO_VGA + i])
				screen[i] = { (i % windowSize.x), (i / windowSize.x) };
			else
				screen[i] = { -1, -1 };
	}

	return 0;
}