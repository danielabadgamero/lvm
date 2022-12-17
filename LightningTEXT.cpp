#include <map>
#include <string>
#include <vector>

#include "LightningCore.h"
#include "LightningTEXT.h"

void Lightning::TEXT::loadFunctions()
{
	commandFunctions['+'] = []()
	{
		if (!command.content.empty())
			if (command.line > 0 && command.line < fileContent.size())
				fileContent.insert(fileContent.begin() + command.line, command.content);
			else
				fileContent.push_back(command.content);
	};

	commandFunctions['-'] = []()
	{
		if (command.line > 0 && command.line < fileContent.size())
			fileContent.erase(fileContent.begin() + command.line);
		else if (fileContent.size() > 0)
			fileContent.pop_back();
	};

	commandFunctions['/'] = []()
	{
		mode = Mode::CMD;
		targetFile = nullptr;
	};
}

bool Lightning::TEXT::parseCommand(std::string* input)
{
	command.cmd = input->substr(0, 1).front();
	std::string lineNum{};
	if (input->size() > 1)
		if (input->at(1) != ' ')
		{
			for (int i{ 1 }; i != input->size(); i++)
				if (input->at(i) == ' ')
				{
					command.content.append(input->substr(1 + i));
					break;
				}
				else
					lineNum.push_back(input->at(i));
			command.line = std::stoi(lineNum);
		}
		else
			command.content.append(input->substr(2));
	return true;
}

void Lightning::TEXT::processCommand()
{
	commandFunctions[command.cmd]();
}