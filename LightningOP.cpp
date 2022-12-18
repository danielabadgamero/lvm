#include <vector>
#include <map>
#include <string>
#include <functional>

#include "LightningOP.h"
#include "LightningCore.h"

void Lightning::OP::loadOperations()
{
	operations[HALT] = []()
	{

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