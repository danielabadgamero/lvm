#include <iostream>
#include <string>
#include <map>

#include "LightningCore.h"

int main()
{
	Lightning::loadFilesystem();

	std::string input{};
	std::string command{};
	std::map<std::string, std::string> arguments{};
	std::string argument{};

	while (Lightning::running)
	{
		Lightning::printPath();
		std::cout << "> ";

		std::getline(std::cin, input);
		if (!input.empty())
		{
			switch (Lightning::mode)
			{
			case Lightning::MODE::CMD:
				command = input;
				for (int i{}; i != input.size(); i++)
					if (input[i] == ' ')
						if (command.empty())
							command = input.substr(0, i);
						else
						{
							size_t mark{ argument.find('=') };
							arguments.emplace(argument.substr(0, mark), argument.substr(mark + 1));
						}
					else if (!command.empty())
						argument.push_back(input[i]);
				break;
			case Lightning::MODE::FILE:
				break;
			}

			Lightning::handleCommand(&command, &arguments);
		}
	}

	return 0;
}