#include "lvm.h"
#include "asm.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc == 1) return 0;
	else if (argc == 2)
	{
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "run") == 0)
		{
			lvm_init(argv[2]);
			while (running) lvm_exec();
			lvm_quit();
		}
		else if (strcmp(argv[1], "asm") == 0)
		{
			asm_build(argv[2]);
		}
	}

	return 0;
}
