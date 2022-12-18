#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningFS.h"

void Lightning::CMD::loadFunctions()
{
	commandFunctions["exit"] = []()
	{
		running = false;
		FS::saveFilesystem();
	};

	commandFunctions["help"] = []()
	{
		for (std::map<std::string, std::function<void()>>::iterator pair{ commandFunctions.begin() }; pair != commandFunctions.end(); pair++)
			std::cout << pair->first << '\t' << commandDescriptions.at(pair->first) << '\n';
	};

	commandFunctions["mkdir"] = []()
	{
		bool valid{ true };
		for (FS::Dir* subDir : FS::path.back()->subDirs)
			if (subDir->name == command.args.at("name"))
				valid = false;
		if (valid)
		{
			FS::path.back()->subDirs.push_back(new FS::Dir{ command.args.at("name") });
			std::sort(FS::path.back()->subDirs.begin(), FS::path.back()->subDirs.end(), [&](FS::Dir* A, FS::Dir* B)
				{
					return A->name < B->name;
				});
		}
	};

	commandFunctions["rmdir"] = []()
	{
		for (std::vector<FS::Dir*>::iterator dir{ FS::path.back()->subDirs.begin() }; dir != FS::path.back()->subDirs.end(); dir++)
			if ((*dir)->name == command.args.at("name"))
			{
				FS::path.back()->subDirs.erase(dir);
				break;
			}
	};

	commandFunctions["cd"] = []()
	{
		if (command.args.at("name") == ".." && FS::path.size() > 1)
			FS::path.pop_back();
		else
			for (int i{}; i != FS::path.back()->subDirs.size(); i++)
				if (FS::path.back()->subDirs.at(i)->name == command.args.at("name"))
				{
					FS::path.push_back(FS::path.back()->subDirs.at(i));
					break;
				}
	};

	commandFunctions["ls"] = []()
	{
		for (FS::Dir* subDir : FS::path.back()->subDirs)
			std::cout << "Dir: " << subDir->name << '\n';
		for (FS::Dir::File file : FS::path.back()->files)
			std::cout << "File: " << file.name << '\n';
	};

	commandFunctions["touch"] = []()
	{
		bool valid{ true };
		for (FS::Dir::File file : FS::path.back()->files)
			if (file.name == command.args.at("name"))
				valid = false;
		if (valid)
		{
			FS::path.back()->files.push_back({ command.args.at("name") });
			std::sort(FS::path.back()->files.begin(), FS::path.back()->files.end(), [&](FS::Dir::File A, FS::Dir::File B)
				{
					return A.name < B.name;
				});
		}
	};

	commandFunctions["rm"] = []()
	{
		for (std::vector<FS::Dir::File>::iterator file{ FS::path.back()->files.begin() }; file != FS::path.back()->files.end(); file++)
			if (file->name == command.args.at("name"))
			{
				FS::path.back()->files.erase(file);
				break;
			}
	};

	commandFunctions["print"] = []()
	{
		FS::Dir::File* file{ nullptr };
		for (std::vector<FS::Dir::File>::iterator f{ FS::path.back()->files.begin() }; f != FS::path.back()->files.end(); f++)
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
		for (std::vector<FS::Dir::File>::iterator f{ FS::path.back()->files.begin() }; f != FS::path.back()->files.end(); f++)
			if (f->name == command.args.at("name"))
				FS::targetFile = &(*f);

		if (FS::targetFile)
		{
			clearScreen();
			mode = Mode::TEXT;
			FS::targetFile->contentVector.clear();
			FS::targetFile->contentVector.push_back("");
			for (char c : FS::targetFile->content)
				if (c == '\n')
					FS::targetFile->contentVector.push_back("");
				else if (FS::targetFile->contentVector.size() != 0)
					FS::targetFile->contentVector.back().push_back(c);
		}
		else
			std::cout << "File not found\n";
	};

	commandFunctions["write"] = []()
	{
		for (std::vector<FS::Dir::File>::iterator f{ FS::path.back()->files.begin() }; f != FS::path.back()->files.end(); f++)
			if (f->name == command.args.at("name"))
				FS::targetFile = &(*f);

		if (FS::targetFile)
		{
			FS::targetFile->contentVector.clear();
			FS::targetFile->contentVector.push_back("");
			for (char c : FS::targetFile->content)
				if (c == '\n')
					FS::targetFile->contentVector.push_back("");
				else if (FS::targetFile->contentVector.size() != 0)
					FS::targetFile->contentVector.back().push_back(c);
			loadProgramme();
		}
		else
			std::cout << "File not found\n";

		FS::targetFile = nullptr;
	};

	commandFunctions["start"] = []()
	{
		for (int i{}; i != loadedProgrammes.size(); i++)
			if (loadedProgrammes.at(i).name == command.args.at("name"))
			{
				addr = RAM + loadedProgrammes.at(i).address;
				mode = Mode::EXEC;
				clearScreen();
				break;
			}
	};

	commandFunctions["prg"] = []()
	{
		for (Programme prg : loadedProgrammes)
			std::cout << prg.name << ":\t" << prg.address << '\n';
	};

	commandFunctions["free"] = []()
	{
		int index{};
		for (int i{}; i != loadedProgrammes.size(); i++)
			if (loadedProgrammes.at(i).name == command.args.at("name"))
				index = i;
		try
		{
			addr = RAM + loadedProgrammes.at(index).address;
			while (addr->allocated)
			{
				addr->value = 0;
				addr->allocated = false;
				addr++;
			}
			loadedProgrammes.erase(loadedProgrammes.begin() + index);
		}
		catch (std::exception e)
		{
			std::cout << e.what() << '\n';
		}
		addr = RAM;
	};

	commandDescriptions.emplace("cd", "Change to a directory in the current path.");
	commandDescriptions.emplace("exit", "Exit the virutal machine saving the current filesystem.");
	commandDescriptions.emplace("free", "Free up all resources allocated for the given programme.");
	commandDescriptions.emplace("help", "This command.");
	commandDescriptions.emplace("ls", "List all files and directories in the current path.");
	commandDescriptions.emplace("mkdir", "Create a directory in the current path.");
	commandDescriptions.emplace("open", "Open a file in the current path and enter TEXT mode.");
	commandDescriptions.emplace("prg", "Show all loaded programmes.");
	commandDescriptions.emplace("print", "Print the contents of a file in the current path.");
	commandDescriptions.emplace("rm", "Remove a file in the current path.");
	commandDescriptions.emplace("rmdir", "Remove a directory in the current path and all of its contents.");
	commandDescriptions.emplace("start", "Execute the programme with the specified name.");
	commandDescriptions.emplace("touch", "Create a file in the current path.");
	commandDescriptions.emplace("write", "Write a programme's instructions to the memory.");
}

bool Lightning::CMD::parseCommand(std::string* input)
{
	// TODO: change std::map<std::string, std::string> command's arguments to std::vector
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