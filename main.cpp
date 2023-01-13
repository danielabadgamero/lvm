#include <SDL.h>

#include "LightningCore.h"

int main(int, char**)
{
	Lightning::init();

	Lightning::cycle();

	Lightning::quit();

	return 0;
}