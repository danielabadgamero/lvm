#include <iostream>

#include "LightningCore.h"

<<<<<<< HEAD
int main()
=======
int main(int, char**)
>>>>>>> gui_2
{
	Lightning::init();

	while (Lightning::running)
	{
<<<<<<< HEAD
		Lightning::CPU.REG[IR] = Lightning::RAM[Lightning::CPU.PC];
		Lightning::CPU.REG[IR] <<= 8;
		Lightning::CPU.REG[IR] += (unsigned char)Lightning::RAM[Lightning::CPU.PC + 1];
		Lightning::CPU.REG[IR] <<= 8;
		Lightning::CPU.REG[IR] += (unsigned char)Lightning::RAM[Lightning::CPU.PC + 2];
		Lightning::CPU.REG[IR] <<= 8;
		Lightning::CPU.REG[IR] += (unsigned char)Lightning::RAM[Lightning::CPU.PC + 3];
		Lightning::CPU.process();
		Lightning::CPU.PC += 4;
	}

=======
		Lightning::processEvents();

		Lightning::draw();
	}

	Lightning::quit();

>>>>>>> gui_2
	return 0;
}