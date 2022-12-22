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
		return nullptr;
	};

	operations[RMEM] = []()
	{
		operation.args[1]->value = RAM[operation.args[0]->value].value;
		return addr + 3;
	};

	operations[WMEM] = []()
	{
		RAM[operation.args[1]->value].value = operation.args[0]->value;
		return addr + 3;
	};

	operations[CALL] = []()
	{
		stack.push(operation.args[0] + 1);
		return &RAM[operation.args[0]->value];
	};

	operations[RET] = []()
	{
		Cell* temp{ stack.top() };
		stack.pop();
		return temp;
	};

	operations[OUT] = []()
	{
		std::cout << static_cast<char>(operation.args[0]->value);
		return addr + 2;
	};
}

bool Lightning::OP::parseOperation()
{
	operation.opcode = static_cast<Opcode>(addr->value);
	operation.args.clear();
	switch (operation.opcode)
	{
	case RMEM:
	case WMEM:
		operation.args.push_back(addr + 1);
		operation.args.push_back(addr + 2);
	case CALL:
	case OUT:
		operation.args.push_back(addr + 1);
	case HALT:
	case RET:
		operation.args.resize(0);
	}

	return true;
}

void Lightning::OP::processOperation()
{
	addr = operations[operation.opcode]();
}