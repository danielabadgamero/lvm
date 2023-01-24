#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

#include <string>

int main(int, char**)
{
	Lightning::init();

	while (~Lightning::CPU::condReg & Lightning::CPU::running)
		Lightning::cycle();

	while (Lightning::CPU::condReg & Lightning::CPU::running)
		Lightning::cycle();

	Lightning::quit();

	return 0;
}