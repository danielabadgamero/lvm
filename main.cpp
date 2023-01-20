#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

#include <string>

int main(int, char** argv)
{
	Lightning::init({ std::atoi(argv[1]), std::atoi(argv[2]) });

	while (Lightning::running)
		Lightning::cycle();

	Lightning::quit();

	return 0;
}