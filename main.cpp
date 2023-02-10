#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

#include <string>

int main(int, char**)
{
	Lightning::init();

	while (true)
		Lightning::cycle();

	Lightning::quit();

	return 0;
}