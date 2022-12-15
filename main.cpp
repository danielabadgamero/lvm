#include <iostream>
#include <fstream>
#include <string>

#include "LightningCore.h"

int main()
{
	std::string command{};

	while (Lightning::running)
	{
		std::cout << "> ";
		std::getline(std::cin, command);
		if (!command.empty())
			Lightning::handleCommand(&command);
	}

	return 0;
}