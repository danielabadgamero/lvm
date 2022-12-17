#include <iostream>
#include <string>
#include <map>

#include "LightningCore.h"

int main()
{
	Lightning::loadFilesystem();

	while (Lightning::running)
	{
		std::string input{};
		std::string command{};
		std::map<std::string, std::string> arguments{};

		switch (Lightning::mode)
		{
		case Lightning::Mode::CMD:
			Lightning::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
			if (!input.empty())
			{
				std::string argument{};
				command = input;
				for (int i{}; i != input.size(); i++)
					if (input[i] == ' ')
						if (command == input)
							command = input.substr(0, i);
						else
						{
							size_t mark{ argument.find('=') };
							arguments.emplace(argument.substr(0, mark), argument.substr(mark + 1));
						}
					else if (command != input)
						argument.push_back(input[i]);
				if (!argument.empty())
				{
					int mark{ (int)argument.find('=') };
					if (mark > 0)
						arguments.emplace(argument.substr(0, mark), argument.substr(mark + 1));
					else
						arguments.emplace("name", argument);
				}
			}
			break;
		case Lightning::Mode::TEXT:
			Lightning::printFileContent();
			Lightning::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			system("cls");
			if (!input.empty())
			{
				command = input.substr(0, 1);
				std::string lineNum{};
				if (input.size() > 1)
					if (input.at(1) != ' ')
					{
						for (int i{ 1 }; i != input.size(); i++)
							if (input.at(i) == ' ')
							{
								arguments.emplace("content", input.substr(1 + i));
								break;
							}
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