#include <vector>
#include <iostream>
#include <string>

#include "LightningCMD.h"
#include "LightningFS.h"

void Lightning::FS::loadFilesystem()
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
					path.push_back(&Filesystem);
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
					path.back()->files.back().contentVector.push_back(content);
					std::getline(fs_in, content);
					content += '\n';
				}
				fs_in.seekg(pos);
				for (int i{}; i != path.back()->files.back().contentVector.size(); i++)
					path.back()->files.back().contentVector.at(i).pop_back();
				path.back()->files.back().contentVector.pop_back();
				path.clear();
				path.push_back(&Filesystem);
			}
		}
	}
}

void Lightning::FS::saveFilesystem()
{
	if (std::filesystem::exists("fs"))
		std::remove("fs");
	fs_out.open("fs", std::ios::binary);
	path.clear();
	writeFilesystem(&Filesystem);

	fs_out.close();
}

void Lightning::FS::writeFilesystem(Dir* dir)
{
	path.push_back(dir);
	for (Dir::File file : dir->files)
	{
		std::string name{ getPath() + file.name + "\n" };
		std::string content{};
		for (std::string str : file.contentVector)
			content.append(str + "\n");
		content.push_back('\n');
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

std::string Lightning::FS::getPath()
{
	std::string pathName{};
	for (Dir* dir : path)
		pathName.append(dir->name + "/");
	return std::string{ pathName };
}

void Lightning::FS::printPath()
{
	for (Dir* dir : path)
		std::cout << dir->name << '/';
	if (targetFile != nullptr)
		std::cout << targetFile->name;
}

void Lightning::FS::printFileContent()
{
	CMD::command.args.emplace("name", targetFile->name);
	CMD::commandFunctions["print"]();
}