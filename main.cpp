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

	while (Lightning::running)
	{
		switch (Lightning::mode)
		{
		case Lightning::MODE::CMD:
			Lightning::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			if (!input.empty())
			{
				std::string argument{};
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
			}
			break;
		case Lightning::MODE::FILE:
			Lightning::printFileContent();
			Lightning::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			if (!input.empty())
			{
				system("cls");
				command = input.substr(0, 1);
				std::string lineNum{};
				if (input.at(1) != ' ')
				{
					for (int i{ 1 }; i != input.size(); i++)
						if (input.at(i) == ' ')
							break;
						else
							lineNum.push_back(input.at(i));
					arguments.emplace("line", lineNum);
				}
				else
					arguments.emplace("content", input.substr(2));
			}
			break;
		}
		
		Lightning::handleCommand(&command, &arguments);
	}

	return 0;
}