#include "Dev.h"
#include "VM.h"

namespace Console
{
	void print_string()
	{
		
	}

	void map(int func)
	{
		
	}
}

void Dev::init()
{
	devices[CONSOLE] = Console::map;
}
