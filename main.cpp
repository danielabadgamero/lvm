#include <iostream>
#include <string>
#include <map>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningTEXT.h"
#include "LightningFS.h"

int main()
{
	Lightning::init();

	while (Lightning::running)
	{
		std::string input{};
		switch (Lightning::mode)
		{
		case Lightning::Mode::CMD:
			Lightning::FS::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			if (!input.empty())
				if (Lightning::CMD::parseCommand(&input))
				{
					try
					{
						Lightning::CMD::processCommand();
					}
					catch (std::exception e)
					{
						std::cout << e.what() << '\n';
					}
				}
			break;
		case Lightning::Mode::TEXT:
			Lightning::FS::printFileContent();
			Lightning::FS::printPath();
			std::cout << "> ";
			std::getline(std::cin, input);
			Lightning::clearScreen();
			if (!input.empty())
				if (Lightning::TEXT::parseCommand(&input))
				{
					try
					{
						Lightning::TEXT::processCommand();
					}
					catch (std::exception e)
					{
						std::cout << e.what() << '\n';
					}
				}
			break;
		case Lightning::Mode::EXEC:
			if (Lightning::addr->allocated)
				switch (static_cast<Lightning::Opcode>(Lightning::addr->value))
				{
				case Lightning::Opcode::HALT:
					Lightning::mode = Lightning::Mode::CMD;
					std::cout << "\n\nProgramme terminated. Press <Enter> to continue...";
					std::getchar();
					break;
				case Lightning::Opcode::ADD:
					break;
				case Lightning::Opcode::COUT:
					Lightning::addr++;
					if ((Lightning::addr + 1)->value == 1)
						std::cout << static_cast<char>(Lightning::addr->value);
					else
						std::cout << Lightning::addr->value;
					Lightning::addr += 2;
					break;
				case Lightning::Opcode::CALL:
					Lightning::addr++;
					Lightning::stack.push(static_cast<int>((Lightning::addr + 1) - Lightning::RAM));
					Lightning::addr = Lightning::RAM + Lightning::addr->value;
					break;
				case Lightning::Opcode::RET:
					Lightning::addr = Lightning::stack.top() + Lightning::RAM;
					Lightning::stack.pop();
					break;
				case Lightning::Opcode::SET:
					Lightning::addr++;
					Lightning::RAM[Lightning::addr->value].value = (Lightning::addr + 1)->value;
					Lightning::addr += 2;
					break;
				case Lightning::Opcode::WMEM:
					Lightning::addr++;
					Lightning::RAM[Lightning::addr->value] = Lightning::RAM[(Lightning::addr + 1)->value];
					Lightning::addr += 2;
					break;
				}
			break;
		}
	}

	return 0;
}