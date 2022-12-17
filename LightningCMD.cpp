#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "LightningCore.h"
#include "LightningCMD.h"

void Lightning::CMD::loadFunctions()
{
	commandFunctions["exit"] = []()
	{
		running = false;
		saveFilesystem();
	};

	commandFunctions["help"] = []()
	{
		for (std::map<std::string, std::function<void()>>::iterator pair{ commandFunctions.begin() }; pair != commandFunctions.end(); pair++)
			std::cout << pair->first << '\t' << commandDescriptions.at(pair->first) << '\n';
	};

	commandFunctions["mkdir"] = []()
	{
		bool valid{ true };
		for (Dir* subDir : path.back()->subDirs)
			if (subDir->name == command.args.at("name"))
				valid = false;
		if (valid)
		{
			path.back()->subDirs.push_back(new Dir{ command.args.at("name") });
			std::sort(path.back()->subDirs.begin(), path.back()->subDirs.end(), [&](Dir* A, Dir* B)
				{
					return A->name < B->name;
				});
		}
	};

	commandFunctions["rmdir"] = []()
	{
		for (std::vector<Dir*>::iterator dir{ path.back()->subDirs.begin() }; dir != path.back()->subDirs.end(); dir++)
			if ((*dir)->name == command.args.at("name"))
			{
				path.back()->subDirs.erase(dir);
				break;
			}
	};

	commandFunctions["cd"] = []()
	{
		if (command.args.at("name") == ".." && path.size() > 1)
			path.pop_back();
		else
			for (int i{}; i != path.back()->subDirs.size(); i++)
				if (path.back()->subDirs.at(i)->name == command.args.at("name"))
				{
					path.push_back(path.back()->subDirs.at(i));
					break;
				}
	};

	commandFunctions["ls"] = []()
	{
		for (Dir* subDir : path.back()->subDirs)
			std::cout << "Dir: " << subDir->name << '\n';
		for (Dir::File file : path.back()->files)
			std::cout << "File: " << file.name << '\n';
	};

	commandFunctions["touch"] = []()
	{
		bool valid{ true };
		for (Dir::File file : path.back()->files)
			if (file.name == command.args.at("name"))
				valid = false;
		if (valid)
		{
			path.back()->files.push_back({ command.args.at("name") });
			std::sort(path.back()->files.begin(), path.back()->files.end(), [&](Dir::File A, Dir::File B)
				{
					return A.name < B.name;
				});
		}
	};

	commandFunctions["rm"] = []()
	{
		for (std::vector<Dir::File>::iterator file{ path.back()->files.begin() }; file != path.back()->files.end(); file++)
			if (file->name == command.args.at("name"))
				path.back()->files.erase(file);
	};

	commandFunctions["print"] = []()
	{
		Dir::File* file{ nullptr };
		for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
			if (f->name == command.args.at("name"))
				file = &(*f);
		int line{ 1 };
		std::cout << line << "   ";
		if (file != nullptr)
			for (std::string::iterator c{ file->content.begin() }; c != file->content.end(); c++)
				if (*c == '\n')
				{
					line++;
					std::cout << '\n' << line;
					if (line < 10)
						std::cout << "   ";
					else if (line < 100)
						std::cout << "  ";
					else
						std::cout << ' ';
				}
				else
					std::cout << *c;
		std::cout << '\n';
	};

	commandFunctions["open"] = []()
	{
		clearScreen();
		for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
			if (f->name == command.args.at("name"))
				targetFile = &(*f);
		mode = Mode::TEXT;

		for (char c : targetFile->content)
			if (c == '\n')
				fileContent.push_back("");
			else if (fileContent.size() != 0)
				fileContent.back().push_back(c);
	};

	commandDescriptions.emplace("cd", "Change to a directory in the current path.");
	commandDescriptions.emplace("exit", "Terminate the programme saving the current filesystem.");
	commandDescriptions.emplace("help", "This command.");
	commandDescriptions.emplace("ls", "List all files and directories in the current path.");
	commandDescriptions.emplace("mkdir", "Create a directory in the current path.");
	commandDescriptions.emplace("open", "Open a file in the current path and enter TEXT mode.");
	commandDescriptions.emplace("print", "Print the contents of a file in the current path.");
	commandDescriptions.emplace("rm", "Remove a file in the current path.");
	commandDescriptions.emplace("rmdir", "Remove a directory in the current path and all of its contents.");
	commandDescriptions.emplace("touch", "Create a file in the current path.");
}

bool Lightning::CMD::parseCommand(std::string* input)
{
	std::string argument{};
	command.cmd = *input;
	command.args.clear();
	for (int i{}; i != input->size(); i++)
		if (input->at(i) == ' ')
			if (command.cmd == *input)
				command.cmd = input->substr(0, i);
			else
			{
				size_t mark{ argument.find('=') };
				command.args.emplace(argument.substr(0, mark), argument.substr(mark + 1));
			}
		else if (command.cmd != *input)
			argument.push_back(input->at(i));
	if (!argument.empty())
	{
		int mark{ (int)argument.find('=') };
		if (mark > 0)
			command.args.emplace(argument.substr(0, mark), argument.substr(mark + 1));
		else
			command.args.emplace("name", argument);
	}
	return true;
}

void Lightning::CMD::processCommand()
{
	commandFunctions[command.cmd]();
}