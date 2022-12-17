#include <iostream>
#include <vector>
#include <string>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningTEXT.h"

void Lightning::loadFilesystem()
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

std::string Lightning::getPath()
{
	std::string pathName{};
	for (Dir* dir : path)
		pathName.append(dir->name + "/");
	return pathName;
}

void Lightning::writeFilesystem(Dir* dir)
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

void Lightning::saveFilesystem()
{
	if (std::filesystem::exists("fs"))
		std::remove("fs");
	fs_out.open("fs", std::ios::binary);
	path.clear();
	writeFilesystem(&FileSystem);

	fs_out.close();
}

std::vector<std::string> Lightning::fileToVector()
{
	std::vector<std::string> lines{ "" };
	for (char c : targetFile->content)
		if (c == '\n')
			lines.push_back("");
		else if (lines.size() != 0)
			lines.back().push_back(c);
	return lines;
}

void Lightning::vectorToFile(std::vector<std::string>* content)
{
	targetFile->content.clear();
	for (std::string str : *content)
		targetFile->content.append(str + '\n');
	if (targetFile->content.size() != 0)
		targetFile->content.pop_back();
}

void Lightning::printPath()
{
	for (Dir* dir : path)
		std::cout << dir->name << '/';
	if (targetFile != nullptr)
		std::cout << targetFile->name;
}

void Lightning::printFileContent()
{
	std::map<std::string, std::string> file{};
	file.emplace("name", targetFile->name);
	CMD::print(&file);
}

void Lightning::handleCommand(std::string* command, std::map<std::string, std::string>* arguments)
{
	switch (mode)
	{
	case MODE::CMD:
		if (*command == "exit")
			CMD::exit(arguments);

		else if (*command == "help")
			CMD::help(arguments);

		else if (*command == "mkdir")
			CMD::mkdir(arguments);

		else if (*command == "rmdir")
			CMD::rmdir(arguments);
		
		else if (*command == "cd")
			CMD::cd(arguments);

		else if (*command == "ls")
			CMD::ls(arguments);

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
		if (!command->empty())
			switch (command->front())
			{
			case '+':
				TEXT::addLine(arguments, &content);
				break;
			case '-':
				TEXT::remLine(arguments, &content);
				break;
			case '/':
				TEXT::close();
				break;
			default:
				std::cout << "Command not found.\n";
			}
		if (targetFile)
			vectorToFile(&content);
		break;
	}
}