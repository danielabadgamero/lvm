#include <SDL.h>

#include "LightningCore.h"

int main(int, char**)
{
	Lightning::init();

	while (Lightning::running)
		Lightning::cycle();

	Lightning::quit();

	return 0;
}