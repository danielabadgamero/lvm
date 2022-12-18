#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <windows.h>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningTEXT.h"
#include "LightningFS.h"
#include "LightningOP.h"

void Lightning::clearScreen()
{
	COORD tl{ 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO s{};
	HANDLE console{ GetStdHandle(STD_OUTPUT_HANDLE) };
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written{};
	DWORD cells{ static_cast<DWORD>(s.dwSize.X * s.dwSize.Y) };
	FillConsoleOutputCharacterW(console, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void Lightning::init()
{
	Lightning::FS::loadFilesystem();
	Lightning::CMD::loadFunctions();
	Lightning::TEXT::loadFunctions();
	Lightning::OP::loadOperations();

	// cout Function
	functions.at("cout").add = static_cast<int>(addr - RAM);
	*addr = { static_cast<int>(Opcode::COUT), true };
	addr++;
	functions.at("cout").args.push_back(static_cast<int>(addr - RAM));
	addr->allocated = true;
	addr++;
	functions.at("cout").args.push_back(static_cast<int>(addr - RAM));
	addr->allocated = true;
	addr++;
	addr->allocated = true;
	addr->value = static_cast<int>(Opcode::RET);
	addr++;
}

void Lightning::loadProgramme()
{
	if (FS::targetFile->contentVector.front() == "LightLang v1.0")
		FS::targetFile->contentVector.erase(FS::targetFile->contentVector.begin());
	else
		return;
	addr = RAM;
	while (addr->allocated || (addr + 1)->allocated)
		addr++;
	std::string prgName{ FS::targetFile->contentVector.front() };
	bool exists{ false };
	for (int i{}; i != loadedProgrammes.size(); i++)
		if (loadedProgrammes.at(i).name == prgName)
		{
			loadedProgrammes.at(i) = { prgName, static_cast<int>(addr - RAM) };
			exists = true;
			break;
		}
	if (!exists)
		loadedProgrammes.push_back({ prgName, static_cast<int>(addr - RAM) });
	FS::targetFile->contentVector.erase(FS::targetFile->contentVector.begin());

	// Do stuff
	
	FS::targetFile->contentVector.insert(FS::targetFile->contentVector.begin(), prgName);
	FS::targetFile->contentVector.insert(FS::targetFile->contentVector.begin(), "LightLang v1.0");
}