#include "Dev.h"
#include "VM.h"

#include <string>
#include <iostream>
#include <unordered_map>

namespace Console
{
	void print_char(unsigned short c)
	{
		std::cout << (char)c << std::flush;
	}

	void get_char(unsigned short addr)
	{
		std::cin.get((char&)VM::RAM[addr]);
	}
}

void Dev::init()
{
	devices[0][0] = Console::print_char;
	devices[0][1] = Console::get_char;
}
