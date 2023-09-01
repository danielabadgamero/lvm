#include "Dev.h"
#include "VM.h"

#include <iostream>

namespace Console
{
	void print_string()
	{
		std::cout << VM::RAM << std::endl;
	}

	void map(int func)
	{
		switch (func)
		{
		case 0x00: print_string(); break;
		}
	}
}

void Dev::init()
{
	devices[CONSOLE] = Console::map;
}
