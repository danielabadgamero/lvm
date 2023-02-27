#include <SDL.h>

#include "LightningCore.h"

int main(int, char**)
{
	Lightning::Core::init();

	while (Lightning::Core::running)
	{
		Lightning::Core::cycle();
	}

	Lightning::Core::quit();

	return 0;
}