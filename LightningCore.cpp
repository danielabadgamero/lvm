#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <windows.h>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningTEXT.h"
#include "LightningFS.h"

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
	std::map<std::string, Symbol> symbols{};
	std::map<std::string, Function> functionSymbols{};
	std::string targetFunction{};
	for (std::string line : FS::targetFile->contentVector)
	{
		std::deque<std::string> words{ "" };
		bool inBrackets{ false };
		for (char c : line)
		{
			if (c == '"')
				inBrackets ^= true;
			if (c == ' ' && !inBrackets)
				words.push_back("");
			else
				words.back().push_back(c);
		}

		if (words.front() == "alloc")
		{
			Cell* currentAddress{ addr };
			addr = &RAM[10000];
			while (addr->allocated)
				addr++;
			words.pop_front();
			symbols.emplace(words.front(), Symbol{ static_cast<int>(addr - RAM), std::stoi(words.at(1)) });
			addr->allocated = true;
			addr->value = std::stoi(words.at(1));
			addr = currentAddress;
		}
		else if (words.front() == "out")
		{
			words.pop_front();
			if (words.front().front() == '"')
			{
				words.front().pop_back();
				words.front().erase(words.front().begin());
				bool escape{};
				bool skipEscape{};
				for (char c : words.front())
				{
					if (escape)
					{
						escape = false;
						switch (c)
						{
						case 'n':
							c = '\n';
							break;
						case '\\':
							skipEscape = true;
							break;
						}
					}

					if (c == '\\' && !skipEscape)
						escape = true;

					if (!escape)
					{
						addr->allocated = true;
						addr->value = static_cast<int>(Opcode::SET);
						addr++;

						addr->allocated = true;
						addr->value = functions.at("cout").args.at(0);
						addr++;

						addr->allocated = true;
						addr->value = static_cast<int>(c);
						addr++;

						addr->allocated = true;
						addr->value = static_cast<int>(Opcode::SET);
						addr++;

						addr->allocated = true;
						addr->value = functions.at("cout").args.at(1);
						addr++;

						addr->allocated = true;
						addr->value = 1;
						addr++;

						addr->allocated = true;
						addr->value = static_cast<int>(Opcode::CALL);
						addr++;

						addr->allocated = true;
						addr->value = functions.at("cout").add;
						addr++;
					}

					skipEscape = false;
				}
			}
			else
			{
				addr->allocated = true;
				addr->value = static_cast<int>(Opcode::WMEM);
				addr++;

				addr->allocated = true;
				addr->value = functions.at("cout").args.at(0);
				addr++;

				addr->allocated = true;
				addr->value = symbols.at(words.front()).add;
				addr++;

				addr->allocated = true;
				addr->value = static_cast<int>(Opcode::SET);
				addr++;

				addr->allocated = true;
				addr->value = functions.at("cout").args.at(1);
				addr++;

				addr->allocated = true;
				addr->value = 0;
				addr++;

				addr->allocated = true;
				addr->value = static_cast<int>(Opcode::CALL);
				addr++;

				addr->allocated = true;
				addr->value = functions.at("cout").add;
				addr++;
			}
		}
		else if (words.front() == "set")
		{
			words.pop_front();
			addr->allocated = true;
			addr->value = static_cast<int>(Opcode::SET);
			addr++;

			addr->allocated = true;
			addr->value = symbols.at(words.front()).add;
			addr++;
			words.pop_front();
			addr->allocated = true;
			addr->value = std::stoi(words.front());
			addr++;
		}
		else if (words.front() == "halt")
		{
			addr->allocated = true;
			addr->value = static_cast<int>(Opcode::HALT);
			addr++;
		}
	}
	FS::targetFile->contentVector.insert(FS::targetFile->contentVector.begin(), prgName);
	FS::targetFile->contentVector.insert(FS::targetFile->contentVector.begin(), "LightLang v1.0");
}