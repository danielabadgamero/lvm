#include <iostream>

#include "LightningCore.h"

int main()
{
	Lightning::init();

	while (Lightning::running)
		Lightning::CPU.process();

	Lightning::quit();

	return 0;
}