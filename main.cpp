#include <iostream>
#include <string>
#include <map>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningTEXT.h"
#include "LightningFS.h"
#include "LightningOP.h"

int main()
{
	Lightning::init();

	while (Lightning::running)
	{
		std::string input{};
		switch (Lightning::mode)
		{
		case Lightning::Mode::CMD:
			Lightning::FS::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			if (!input.empty())
				if (Lightning::CMD::parseCommand(&input))
				{
					try
					{
						Lightning::CMD::processCommand();
					}
					catch (std::exception e)
					{
						std::cout << e.what() << '\n';
					}
				}
			break;
		case Lightning::Mode::TEXT:
			Lightning::FS::printFileContent();
			Lightning::FS::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			Lightning::clearScreen();
			if (!input.empty())
				if (Lightning::TEXT::parseCommand(&input))
				{
					try
					{
						Lightning::TEXT::processCommand();
					}
					catch (std::exception e)
					{
						std::cout << e.what() << '\n';
					}
				}
			break;
		case Lightning::Mode::EXEC:
			if (Lightning::addr->allocated)
				if (Lightning::OP::parseOperation())
					Lightning::OP::proccessOperation();
			break;
		}
	}

	return 0;
}