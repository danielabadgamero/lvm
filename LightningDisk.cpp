#include <string>
#include <filesystem>
#include <fstream>

#include <SDL.h>

#include "LightningCPU.h"
#include "LightningDisk.h"

int Lightning::Disk::cycle(void*)
{
	for (const auto& dirEntry : std::filesystem::directory_iterator("fs"))
	{
		std::string filename{ dirEntry.path().filename().string() };
		if (filename == "boot")
		{
			std::ifstream boot{ dirEntry.path(), std::ifstream::binary };
			boot.read((char*)HDD[0], 512);
		}
	}

	while (~CPU::bistables[CPU::running]);

	while (CPU::bistables[CPU::running])
	{
		if (*rRead)
		{

		}
	}

	return 0;
}