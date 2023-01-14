#include <SDL.h>

#include "LightningCore.h"

int main(int, char* argv[])
{
	SDL_Point windowSize{ std::atoi(argv[1]), std::atoi(argv[2]) };
	Lightning::init(windowSize);

	while (Lightning::running)
		Lightning::cycle();

	Lightning::quit();

	return 0;
}