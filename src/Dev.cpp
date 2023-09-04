#include "Dev.h"
#include "VM.h"

#include <string>
#include <iostream>
#include <unordered_map>

namespace Console
{
	void print_char()
	{
		std::cout << VM::RAM[0] << std::flush;
	}

	void print_string()
	{
		std::cout << VM::RAM << std::flush;
	}

	void print_stringln()
	{
		std::cout << VM::RAM << std::endl;
	}
	
	void new_line()
	{
		std::cout << std::endl;
	}

	void get_char()
	{
		std::cin.get((char*)(VM::RAM))
	}

	void map(int func)
	{
		switch (func)
		{
		case 0x00: print_char(); break;
		case 0x01: print_string(); break;
		case 0x02: print_stringln(); break;
		case 0x03: new_line(); break;
		case 0x04: get_char(); break;
		}
	}
}

void Dev::init()
{
	devices[CONSOLE] = Console::map;
}
