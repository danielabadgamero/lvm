#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>

#include "LightningCMD.h"
#include "LightningTEXT.h"

namespace Lightning
{
	enum class OP
	{
		HALT,
	};

	enum class MODE
	{
		CMD,
		TEXT,
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
	MODE mode{ MODE::CMD };

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

	std::vector<std::string> fileToVector()
	{
		std::vector<std::string> lines{ "" };
		for (char c : targetFile->content)
			if (c == '\n')
				lines.push_back("");
			else
				lines.back().push_back(c);
		return lines;
	}

	void vectorToFile(std::vector<std::string>* content)
	{
		targetFile->content.clear();
		for (std::string str : *content)
			targetFile->content.append(str + '\n');
		targetFile->content.pop_back();
	}

	void printPath()
	{
		for (Dir* dir : path)
			std::cout << dir->name << '/';
		if (targetFile != nullptr)
			std::cout << targetFile->name;
	}

	void printFileContent()
	{
		CMD::print(new std::map<std::string, std::string>{ std::string("name"), targetFile->name });
	}

	void handleCommand(std::string* command, std::map<std::string, std::string>* arguments)
	{
		switch (mode)
		{
		case MODE::CMD:
			if (*command == "exit")
				CMD::exit();

			else if (*command == "help")
				CMD::help(arguments);

			else if (*command == "mkdir")
				CMD::mkdir(arguments);

			else if (*command == "rmdir")
				CMD::rmdir(arguments);

			else if (*command == "cd")
				CMD::cd(arguments);

			else if (*command == "ls")
				CMD::ls();

			else if (*command == "touch")
				CMD::touch(arguments);

			else if (*command == "rm")
				CMD::rm(arguments);

			else if (*command == "print")
				CMD::print(arguments);

			else if (*command == "open")
				CMD::open(arguments);

			else
				std::cout << "Command not found.\n";

			break;

		case MODE::TEXT:
			std::vector<std::string> content{ fileToVector() };
			switch (command->front())
			{
			case '+':
				TEXT::addLine(arguments, &content);
				break;
			case '-':
				TEXT::remLine(arguments, &content);
				break;
			}
			vectorToFile(&content);
			break;
		}
	}
}

#endif