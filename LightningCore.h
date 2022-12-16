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
	Dir::File* targetFile{ nullptr };

	int* addr{ RAM };
	bool running{ true };

	void loadFilesystem()
	{
		if (std::filesystem::exists("fs"))
		{
			fs_in.open("fs", std::ios::binary);
			std::vector<std::string> dirPaths{};
			bool nextContent{ false };
			while (!fs_in.eof() && !fs_in.fail())
			{
				if (!nextContent)
				{
					dirPaths.push_back("");
					std::getline(fs_in, dirPaths.back());
					std::vector<std::string> dirs{};
					for (char c : dirPaths.back())
						if (c == '/')
							dirs.push_back("");
						else
							dirs.back().push_back(c);
					for (std::string targetDir : dirs)
					{
						if (!targetDir.empty() && targetDir != dirs.back())
						{
							bool exists{ false };
							for (std::vector<Dir*>::iterator subDir{ path.back()->subDirs.begin() }; subDir != path.back()->subDirs.end(); subDir++)
								if ((*subDir)->name == targetDir)
								{
									exists = true;
									path.push_back(*subDir);
									break;
								}
							if (!exists)
							{
								path.back()->subDirs.push_back(new Dir{ targetDir });
								path.push_back(path.back()->subDirs.back());
							}
						}
						if (!dirs.back().empty() && targetDir == dirs.back())
						{
							path.back()->files.push_back({ targetDir });
							nextContent = true;
						}
					}
					if (!nextContent)
					{
						path.clear();
						path.push_back(&FileSystem);
					}
				}
				else
				{
					nextContent = false;
					std::string content{};
					std::streampos pos{ fs_in.tellg() };
					std::getline(fs_in, content);
					content += '\n';
					while (content.front() != '/' && !fs_in.eof())
					{
						pos = fs_in.tellg();
						path.back()->files.back().content += content;
						std::getline(fs_in, content);
						content += '\n';
					}
					fs_in.seekg(pos);
					path.back()->files.back().content.pop_back();
					path.clear();
					path.push_back(&FileSystem);
				}
			}
		}
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
		if (targetFile != nullptr)
			std::cout << targetFile->name;
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
		else if (cmd == "rmdir")
		{
			if (!arg.empty())
			{
				for (std::vector<Dir*>::iterator dir{ path.back()->subDirs.begin() }; dir != path.back()->subDirs.end(); dir++)
					if ((*dir)->name == arg)
					{
						path.back()->subDirs.erase(dir);
						break;
					}
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "cd")
		{
			if (!arg.empty())
			{
				targetFile = nullptr;
				Dir* target{ nullptr };
				if (arg == ".." && path.size() > 1)
				{
					path.pop_back();
					if (path.size() == 1)
						target = &FileSystem;
				}
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
		else if (cmd == "touch")
		{
			if (!arg.empty())
			{
				bool valid{ true };
				for (Dir::File file : path.back()->files)
					if (file.name == arg)
					{
						valid = false;
						break;
					}
				if (valid)
				{
					path.back()->files.push_back({ arg });
					std::sort(path.back()->files.begin(), path.back()->files.end(), [&](Dir::File A, Dir::File B)
						{
							return A.name < B.name;
						});
				}
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "rm")
		{
			if (!arg.empty())
			{
				for (std::vector<Dir::File>::iterator file{ path.back()->files.begin() }; file != path.back()->files.end(); file++)
					if (file->name == arg)
					{
						path.back()->files.erase(file);
						break;
					}
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "print")
		{
			if (!arg.empty() || (targetFile != nullptr))
			{
				Dir::File* file{ targetFile };
				for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
					if (f->name == arg)
					{
						file = &(*f);
						break;
					}
				if (file != nullptr)
				{
					int line{ 1 };
					std::cout << line << "   ";
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
				}
				else
					std::cout << "Error: Couldn't find file with name \"" << arg << "\"\n";
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd == "open")
		{
			if (!arg.empty())
			{
				Dir::File* file{ nullptr };
				for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
					if (f->name == arg)
					{
						file = &(*f);
						break;
					}
				if (file != nullptr)
				{
					targetFile = file;
				}
				else
					std::cout << "Error: Couldn't find file with name \"" << arg << "\"\n";
			}
			else
				printUnknown(&cmd, &arg);
		}
		else if (cmd.front() == '/' && (targetFile != nullptr))
		{
			cmd.erase(cmd.begin());
			targetFile->content.append(cmd + " " + arg + "\n");
		}
		else if (cmd.front() == '-' && (targetFile != nullptr))
		{
			std::vector<std::string> lines{""};
			for (char c : targetFile->content)
				if (c == '\n')
					lines.push_back("");
				else
					lines.back().push_back(c);
			if (cmd.size() == 1)
				lines.pop_back();
			else
			{
				int numLine{ std::stoi(cmd.substr(1)) - 1 };
				std::vector<std::string>::iterator line{ lines.begin() + numLine };
				lines.erase(line);
			}
			targetFile->content.clear();
			if (lines.size() > 0)
			{
				for (std::string str : lines)
					targetFile->content.append(str + '\n');
				targetFile->content.pop_back();
			}
		}
		else if (cmd.front() == '+' && (targetFile != nullptr))
		{
			std::vector<std::string> lines{ "" };
			for (char c : targetFile->content)
				if (c == '\n')
					lines.push_back("");
				else
					lines.back().push_back(c);
			
			int numLine{ std::stoi(cmd.substr(1)) - 1 };
			std::vector<std::string>::iterator line{ lines.begin() + numLine };
			lines.insert(line, arg);
			
			targetFile->content.clear();
			for (std::string str : lines)
				targetFile->content.append(str + '\n');
			targetFile->content.pop_back();
		}
		else
			printUnknown(&cmd, &arg);
	}
}

#endif