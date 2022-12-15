#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>

namespace Lightning
{
	enum class OP
	{
		HALT,
	};

	struct Dir
	{
		struct File
		{
			std::string name{};
		};

		std::string name{};
		std::vector<File> files{};
		std::vector<Dir*> subDirs{};
	};

	int RAM[1 << 15];
	int REG[8];
	std::ifstream fs_in{};
	std::ofstream fs_out{};
	Dir FileSystem{};
	std::vector<Dir*> path{};

	int* addr{ RAM };
	bool running{ true };

	void loadFileSystem()
	{
		if (std::filesystem::exists("fs"))
		{
			fs_in.open("fs", std::ios::binary);
		}
		else
		{
			path.push_back(&FileSystem);
		}
	}

	void saveFileSystem()
	{
		if (std::filesystem::exists("fs"))
			std::remove("fs");
		fs_out.open("fs", std::ios::binary);
	}

	void printPath()
	{
		for (Dir* dir : path)
			std::cout << dir->name << '/';
	}

	void printUnknown(std::string* cmd, std::string* arg)
	{
		std::cout << "\n\rUnknown command: \"" << *cmd << ' ' << *arg <<
			"\". Please type \"help all\" to see a list of all available commands, or \"help <command>\" to see the usage of a specific command.\n\n";
	}

	void handleCommand(std::string* command)
	{
		int argSep{ static_cast<int>(command->find(' ')) };
		std::string cmd{ *command };
		std::string arg{};
		if (argSep >= 0)
		{
			arg = command->substr(argSep + 1);
			cmd = command->substr(0, argSep);
		}

		if (cmd == "exit")
		{
			running = false;
			saveFileSystem();
		}
		else if (cmd == "help")
		{
			if (arg == "all")
				std::cout << "\n\rhelp <command>: \tthis command.\n" <<
				"exit: \t\t\texit from the VM and terminate all processes.\n\n";
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "mkdir")
		{
			if (!arg.empty())
			{
				bool valid{ true };
				for (Dir* subDir : path.back()->subDirs)
					if (subDir->name == arg)
					{
						std::cout << "Could not create directory \"" << arg << "\": directory already exists.\n";
						valid = false;
						break;
					}
				if (valid)
				{
					path.back()->subDirs.push_back(new Dir{ arg });
					std::sort(path.back()->subDirs.begin(), path.back()->subDirs.end(), [&](Dir* A, Dir* B)
						{
							return A->name < B->name;
						});
				}
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "cd")
		{
			if (!arg.empty())
			{
				Dir* target{ nullptr };
				if (arg == ".." && path.size() > 1)
					path.pop_back();
				else
				{
					for (std::vector<Dir*>::iterator i{ path.back()->subDirs.begin() }; i != path.back()->subDirs.end(); i++)
						if ((*i)->name == arg)
						{
							target = *i;
							path.push_back(target);
							break;
						}
					if (target == nullptr)
						std::cout << "Error: Directory name not found.\n";
				}
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "ls")
		{
			for (Dir* subDir : path.back()->subDirs)
				std::cout << "Dir: " << subDir->name << '\n';
			for (Dir::File file : path.back()->files)
				std::cout << "File: " << file.name << '\n';
		}
		else
			printUnknown(&cmd, &arg);
	}
}

#endif