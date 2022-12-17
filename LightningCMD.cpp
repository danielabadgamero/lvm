#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "LightningCore.h"
#include "LightningCMD.h"

void Lightning::CMD::exit(std::map<std::string, std::string>* arguments)
{
	arguments;
	running = false;
	saveFilesystem();
}

void Lightning::CMD::help(std::map<std::string, std::string>* arguments)
{
	if (arguments->empty())
	{
		// TODO: write all commands
	}
	else
	{
		std::string command{ arguments->at("command") };
		if (!command.empty())
		{
			// TODO: write specific command syntax
		}
	}
}

void Lightning::CMD::mkdir(std::map<std::string, std::string>* arguments)
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
}

void Lightning::CMD::rmdir(std::map<std::string, std::string>* arguments)
{
	std::string dirName{ arguments->at("name") };

	for (std::vector<Dir*>::iterator dir{ path.back()->subDirs.begin() }; dir != path.back()->subDirs.end(); dir++)
		if ((*dir)->name == dirName)
		{
			path.back()->subDirs.erase(dir);
			break;
		}
}

void Lightning::CMD::cd(std::map<std::string, std::string>* arguments)
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
}

void Lightning::CMD::ls(std::map<std::string, std::string>* arguments)
{
	arguments;
	for (Dir* subDir : path.back()->subDirs)
		std::cout << "Dir: " << subDir->name << '\n';
	for (Dir::File file : path.back()->files)
		std::cout << "File: " << file.name << '\n';
}

void Lightning::CMD::touch(std::map<std::string, std::string>* arguments)
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
}

void Lightning::CMD::rm(std::map<std::string, std::string>* arguments)
{
	std::string fileName{ arguments->at("name") };

	for (std::vector<Dir::File>::iterator file{ path.back()->files.begin() }; file != path.back()->files.end(); file++)
		if (file->name == fileName)
		{
			path.back()->files.erase(file);
			break;
		}
}

void Lightning::CMD::print(std::map<std::string, std::string>* arguments)
{
	std::string fileName{ arguments->at("name") };

	Dir::File* file{};
	for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
		if (f->name == fileName)
		{
			file = &(*f);
			break;
		}

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

void Lightning::CMD::open(std::map<std::string, std::string>* arguments)
{
	system("cls");
	std::string fileName{ arguments->at("name") };

	Dir::File* file{ nullptr };
	for (std::vector<Dir::File>::iterator f{ path.back()->files.begin() }; f != path.back()->files.end(); f++)
		if (f->name == fileName)
		{
			file = &(*f);
			break;
		}

	targetFile = file;
	mode = MODE::TEXT;
}