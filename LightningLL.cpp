#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>

#include "LightningLL.h"
#include "LightningCore.h"
#include "LightningOP.h"

void Lightning::LL::compile(std::vector<std::string>* source, std::vector<std::string>* binary)
{
	bin = binary;
	bin->insert(bin->begin(), "%%");
	for (std::vector<std::string>::iterator i{ source->begin() }; i != source->end(); i++)
	{
		line = *i;
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

		int space{ static_cast<int>(line.find(' ')) };
		if (space > 0)
		{
			line = line.substr(static_cast<size_t>(space) + 1);
		}
		std::string keyword{ line.substr(0, line.find(' ')) };
		keywords[keyword]();
	}
}

void Lightning::LL::loadFunctions()
{
	keywords["func"] = []()
	{
		std::string symbol{};
	};
}