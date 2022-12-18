#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <functional>

#include "LightningOP.h"
#include "LightningCore.h"

void Lightning::OP::loadOperations()
{
	operations[HALT] = []()
	{
		mode = Mode::CMD;
		std::cout << "\nProgramme terminated. Press <Enter> to return to CMD mode.\n";

	};

	operations[SET] = []()
	{

	};

	operations[GET] = []()
	{

	};

	operations[RMEM] = []()
	{

	};

	operations[WMEM] = []()
	{

	};

	operations[CALL] = []()
	{

	};

	operations[RET] = []()
	{

	};

	operations[ADD] = []()
	{

	};

	operations[OUT] = []()
	{

	};
}

bool Lightning::OP::parseOperation()
{

}

void Lightning::OP::processOperation()
{

}