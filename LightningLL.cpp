#include <string>
#include <vector>
#include <deque>
#include <stdexcept>

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
	addr = os_func_start;

	advanceAddr(4);
	addr->value = OP::WMEM;
	functions["set"].address = addr;
	addr++;
	functions["set"].args[0] = addr;
	addr++;
	functions["set"].args[1] = addr;
	addr++;
	addr->value = OP::RET;
	addr++;

	advanceAddr(0);
	addr->value = 
}

void Lightning::LL::compile(std::vector<std::string>* content)
{
	int sections[2]
	{
		std::distance(content->begin(), std::find(content->begin(), content->end(), "var:")) + 1,
		std::distance(content->begin(), std::find(content->begin(), content->end(), "src:")) + 1
	};
	std::vector<std::string>::iterator line{ content->begin() };
	line += sections[0];
	addr = symb_start;
	while (line != (line + sections[1]))
	{
		std::string name{ line->substr(0, line->find(' ')) };
		std::string value{ line->substr(line->find(' ')) };
		symbols.emplace(name);
		symbols.at(name) = addr - RAM;
		try
		{
			int val{ std::stoi(value) };
			advanceAddr(1);
			addr->value = val;
			addr++;
		}
		catch (std::invalid_argument)
		{
			advanceAddr(value.size());
			for (char c : value)
			{
				addr->value = static_cast<int>(c);
				addr++;
			}
		}
	}
	addr = prog_start;
	line++;
	while (line != content->end())
	{
		std::vector<std::string> words{ "" };
		for (char c : *line)
			if (c == ' ')
				words.push_back("");
			else
				words.back().push_back(c);
		words.resize(3);

		advanceAddr(8);
		addr->value = OP::WMEM;
		addr++;
		addr->value = static_cast<int>(functions[words[0]].args[0] - RAM);
		addr++;
		addr->value = symbols.at(words[1]);
		addr++;
		addr->value = OP::WMEM;
		addr++;
		addr->value = static_cast<int>(functions[words[0]].args[1] - RAM);
		addr++;
		addr->value = std::stoi(words[2]);
		addr++;
		addr->value = OP::CALL;
		addr++;
		addr->value = static_cast<int>(functions[words[0]].address - RAM);
		addr++;
	}
}