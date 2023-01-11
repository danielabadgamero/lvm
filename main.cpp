#include <iostream>

#include "LightningCore.h"

int main(int, char**)
{
	Lightning::init();

	while (Lightning::running)
	{
		Lightning::processEvents();

		Lightning::draw();
	}

	Lightning::quit();

	return 0;
}