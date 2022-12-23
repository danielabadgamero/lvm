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
#include "LightningLL.h"

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
	Lightning::LL::loadFunctions();
	Lightning::OP::loadOperations();
}

void Lightning::loadProgramme()
{
	if (FS::targetFile->contentVector.front() == "LightLang v1.1")
		FS::targetFile->contentVector.erase(FS::targetFile->contentVector.begin());
	else
		return;

	std::vector<std::string>* bin{ &FS::path.back()->files.back().contentVector };
	LL::compile(&FS::targetFile->contentVector, bin);
	
	FS::targetFile->contentVector.insert(FS::targetFile->contentVector.begin(), "LightLang v1.1");
}