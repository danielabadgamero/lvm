#include <iostream>
#include <string>

#include "LightningCore.h"

int main()
{
	Lightning::loadFilesystem();

	std::string command{};

	while (Lightning::running)
	{
		Lightning::printPath();
		std::cout << "> ";
		std::getline(std::cin, command);
		if (!command.empty())
			Lightning::handleCommand(&command);
	}

	return 0;
}