#include <string>
#include <vector>
#include <deque>

#include "LightningLL.h"
#include "LightningCore.h"
#include "LightningOP.h"

static void alloc(Lightning::Cell* begin, int places)
{
	for (int i{}; i != places; i++)
		(begin + i)->allocated = true;
}

static void advanceAddr(int freeCells)
{
	bool avail{ true };
	do
		for (int i{}; i != freeCells; i++)
			if ((Lightning::addr + i)->allocated)
			{
				avail = false;
				Lightning::addr += i;
				break;
			}
			else if (i == freeCells - 1)
				avail = true;
	while (!avail);

	alloc(Lightning::addr, freeCells);
}

void Lightning::LL::loadFunctions()
{
	addr = RAM;

	advanceAddr(0);
	
}

void Lightning::LL::compile(std::vector<std::string>* content)
{
	for (std::string line : *content)
	{

	}
}