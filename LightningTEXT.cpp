#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "LightningCore.h"
#include "LightningTEXT.h"
#include "LightningFS.h"

void Lightning::TEXT::loadFunctions()
{
	commandFunctions['+'] = []()
	{
		if (!command.content.empty())
			if (command.line >= 0 && command.line < FS::targetFile->contentVector.size())
				FS::targetFile->contentVector.insert(FS::targetFile->contentVector.begin() + command.line, command.content);
			else
				FS::targetFile->contentVector.push_back(command.content);
	};

	commandFunctions['-'] = []()
	{
		if (command.line >= 0 && command.line < FS::targetFile->contentVector.size())
			FS::targetFile->contentVector.erase(FS::targetFile->contentVector.begin() + command.line);
		else if (FS::targetFile->contentVector.size() > 0)
			FS::targetFile->contentVector.pop_back();
	};

	commandFunctions['/'] = []()
	{
		mode = Mode::CMD;
		FS::targetFile = nullptr;
	};

	commandFunctions['?'] = []()
	{
		for (std::map<char, std::function<void()>>::iterator pair{ commandFunctions.begin() }; pair != commandFunctions.end(); pair++)
			std::cout << pair->first << '\t' << commandDescriptions.at(pair->first) << '\n';
	};

	commandDescriptions.emplace('?', "This command.");
	commandDescriptions.emplace('+', "Insert before line number, or at bottom if none provided.");
	commandDescriptions.emplace('-', "Revome line number, or last one if none provided.");
	commandDescriptions.emplace('/', "Save file and exit.");
}

bool Lightning::TEXT::parseCommand(std::string* input)
{
	command.cmd = input->substr(0, 1).front();
	command.content.clear();
	command.line = -1;
	if (command.cmd != '+' && command.cmd != '-' && command.cmd != '?' && command.cmd != '/') // Fix this awful looking line
	{
		command.cmd = '+';
		command.content = *input;
		return true;
	}
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
			try
			{
				command.line = std::stoi(lineNum) - 1;
			}
			catch (std::exception e)
			{
				std::cout << e.what() << '\n';
				return false;
			}
		}
		else
			command.content.append(input->substr(2));
	return true;
}

void Lightning::TEXT::processCommand()
{
	commandFunctions[command.cmd]();
}