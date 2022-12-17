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
	commandFunctions.at("exit") = [](Arguments arguments)
	{
		arguments;
		running = false;
		saveFilesystem();
	};

	commandFunctions["help"] = [](Arguments arguments)
	{
		if (arguments->empty())
			for (std::map<std::string, std::function<void(Arguments)>>::iterator pair{ commandFunctions.begin() }; pair != commandFunctions.end(); pair++)
				std::cout << pair->first << '\t' << commandDescriptions.at(pair->first) << '\n';
		else
		{
			std::string command{ arguments->at("command") };
			if (!command.empty())
			{
				// TODO: write specific command syntax
			}
		}
	};

	commandFunctions["mkdir"] = [](Arguments arguments)
	{
		std::string dirName{ arguments->at("name") };
		bool valid{ true };
		for (Dir* subDir : path.back()->subDirs)
			if (subDir->name == dirName)
			{
				valid = false;
				break;
			}
		if (valid)
		{
			path.back()->subDirs.push_back(new Dir{ dirName });
			std::sort(path.back()->subDirs.begin(), path.back()->subDirs.end(), [&](Dir* A, Dir* B)
				{
					return A->name < B->name;
				});
		}
	};

	commandFunctions["rmdir"] = [](Arguments arguments)
	{
		std::string dirName{ arguments->at("name") };

		for (std::vector<Dir*>::iterator dir{ path.back()->subDirs.begin() }; dir != path.back()->subDirs.end(); dir++)
			if ((*dir)->name == dirName)
			{
				path.back()->subDirs.erase(dir);
				break;
			}
	};

	commandFunctions["cd"] = [](Arguments arguments)
	{
		std::string dirName{ arguments->at("name") };

		if (dirName == ".." && path.size() > 1)
			path.pop_back();
		else
			for (std::vector<Dir*>::iterator i{ path.back()->subDirs.begin() }; i != path.back()->subDirs.end(); i++)
				if ((*i)->name == dirName)
				{
					path.push_back(*i);
					break;
				}
	};

	commandFunctions["ls"] = [](Arguments arguments)
	{
		arguments;
		for (Dir* subDir : path.back()->subDirs)
			std::cout << "Dir: " << subDir->name << '\n';
		for (Dir::File file : path.back()->files)
			std::cout << "File: " << file.name << '\n';
	};

	commandFunctions["touch"] = [](Arguments arguments)
	{
		std::string fileName{ arguments->at("name") };

		bool valid{ true };
		for (Dir::File file : path.back()->files)
			if (file.name == fileName)
			{
				valid = false;
				break;
			}
		if (valid)
		{
			path.back()->files.push_back({ fileName });
			std::sort(path.back()->files.begin(), path.back()->files.end(), [&](Dir::File A, Dir::File B)
				{
					return A.name < B.name;
				});
		}
	};

	commandFunctions["rm"] = [](Arguments arguments)
	{
		std::string fileName{ arguments->at("name") };

		for (std::vector<Dir::File>::iterator file{ path.back()->files.begin() }; file != path.back()->files.end(); file++)
			if (file->name == fileName)
			{
				path.back()->files.erase(file);
				break;
			}
	};

	commandFunctions["print"] = [](Arguments arguments)
	{
		std::string fileName{ arguments->at("name") };

		Dir::File* file{ nullptr };
		for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
			if (f->name == fileName)
			{
				file = &(*f);
				break;
			}

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

	commandFunctions["open"] = [](Arguments arguments)
	{
		clearScreen();
		std::string fileName{ arguments->at("name") };

		Dir::File* file{ nullptr };
		for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
			if (f->name == fileName)
			{
				file = &(*f);
				break;
			}

		targetFile = file;
		mode = Mode::TEXT;
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