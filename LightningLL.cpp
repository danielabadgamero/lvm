#include <string>
#include <vector>
#include <deque>
#include <stdexcept>
#include <iostream>

#include "LightningLL.h"
#include "LightningCore.h"
#include "LightningOP.h"

static inline constexpr int VAR{ 0 };
static inline constexpr int SRC{ 1 };

void Lightning::LL::compile(std::vector<std::string>* source, std::vector<std::string>* bin)
{
	std::vector<std::string>::iterator sections[2]
	{
		std::find(source->begin(), source->end(), "var:") + 1,
		std::find(source->begin(), source->end(), "src:") + 1
	};
	int size{};
	for (auto line{ sections[VAR] }; line != sections[SRC] - 1; line++)
	{
		try
		{
			symbols.emplace(line->substr(0, line->find(' ')), std::stoi(line->substr(line->find(' ') + 1)));
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid variable initializer value at line " << line - source->begin() << ": " << *line << '\n';
			return;
		}
	}
	for (std::map<std::string, int>::iterator symbol{ symbols.begin() }; symbol != symbols.end(); symbol++)
	{
		bin->push_back("");
		bin->back().append(symbol->first + " " + std::to_string(symbol->second));
		size++;
	}
	bin->push_back("%%");



	bin->insert(bin->begin(), std::to_string(size));
}