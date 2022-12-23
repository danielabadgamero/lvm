#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <conio.h>

#include "LightningOP.h"
#include "LightningCore.h"

void Lightning::OP::loadOperations()
{
	operations[HALT] = []()
	{
		mode = Mode::CMD;
		addr = RAM;
		while (addr->allocated)
		{
			addr->value = 0;
			addr->allocated = false;
			addr++;
		}
		std::cout << "\n\n";
		system("pause");
		return RAM;
	};

	operations[RWMEM] = []()
	{
		(addr + operation.args[1]->value)->value = RAM[operation.args[0]->value].value;
		return addr + 3;
	};

	operations[RMEM] = []()
	{
		operation.args[1]->value = RAM[operation.args[0]->value].value;
		return addr + 3;
	};

	operations[WMEM] = []()
	{
		RAM[operation.args[0]->value].value = operation.args[1]->value;
		return addr + 3;
	};

	operations[OUT] = []()
	{
		std::cout << operation.args[0]->value << '\n';
		return addr + 2;
	};
}

bool Lightning::OP::parseOperation()
{
	operation.opcode = static_cast<Opcode>(addr->value);
	operation.args.clear();
	switch (operation.opcode)
	{
	case RWMEM:
	case RMEM:
	case WMEM:
		operation.args.push_back(addr + 1);
		operation.args.push_back(addr + 2);
		break;
	case OUT:
		operation.args.push_back(addr + 1);
		break;
	case HALT:
		operation.args.resize(0);
		break;
	}

	return true;
}

void Lightning::OP::processOperation()
{
	addr = operations[operation.opcode]();
}