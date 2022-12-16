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
			std::string content{};
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
	std::vector<Dir*> path{ &FileSystem };

	int* addr{ RAM };
	bool running{ true };

	void loadFilesystem()
	{
	}

	std::string getPath()
	{
		std::string pathName{};
		for (Dir* dir : path)
			pathName.append(dir->name + "/");
		return pathName;
	}

	void writeFilesystem(Dir* dir)
	{
		path.push_back(dir);
		for (Dir::File file : dir->files)
		{
			std::string name{ getPath() + file.name + "\n" };
			std::string content{ file.content + "\n" };
			fs_out.write(name.c_str(), name.size());
			fs_out.write(content.c_str(), content.size());
		}
		for (Dir* subDir : dir->subDirs)
		{
			std::string name{ getPath() + subDir->name + "/\n" };
			fs_out.write(name.c_str(), name.size());
			writeFilesystem(subDir);
		}
		path.pop_back();
	}

	void saveFilesystem()
	{
		if (std::filesystem::exists("fs"))
			std::remove("fs");
		fs_out.open("fs", std::ios::binary);
		path.clear();
		writeFilesystem(&FileSystem);

		fs_out.close();
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
			saveFilesystem();
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