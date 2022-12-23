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
	}
	bin->push_back("%%");

	for (auto line{ sections[SRC] }; line != source->end(); line++)
	{
		function.args.resize(static_cast<int>(std::count(line->begin(), line->end(), ' ')));
		function.name = *line;
		if (function.args.size() > 0)
		{
			size_t space{ line->find(' ') };
			function.name = line->substr(0, space);
			function.args[0] = line->substr(space + 1);
			if (function.args.size() > 1)
			{
				function.args[1] = line->substr(line->find(' ', space + 1) + 1);
				function.args[0] = function.args[0].substr(0, function.args[0].find(' '));
			}
		}

		functions[function.name](bin);
	}
}

void Lightning::LL::loadFunctions()
{
	functions["set"] = [](std::vector<std::string>* bin)
	{
		try
		{
			int n{ std::stoi(function.args[1]) };
			bin->push_back(std::to_string(OP::WMEM));
			bin->push_back(function.args[0]);
			bin->push_back(std::to_string(n));
		}
		catch (std::invalid_argument)
		{
			bin->push_back(std::to_string(OP::RWMEM));
			bin->push_back(function.args[1]);
			bin->push_back(std::to_string(5));
			bin->push_back(std::to_string(OP::WMEM));
			bin->push_back(function.args[0]);
			bin->push_back(std::to_string(0));
		}
	};

	functions["print"] = [](std::vector<std::string>* bin)
	{
		try
		{
			int n{ std::stoi(function.args[0]) };
			bin->push_back(std::to_string(OP::OUT));
			bin->push_back(std::to_string(n));
		}
		catch (std::invalid_argument)
		{
			bin->push_back(std::to_string(OP::RWMEM));
			bin->push_back(function.args[0]);
			bin->push_back(std::to_string(4));
			bin->push_back(std::to_string(OP::OUT));
			bin->push_back(std::to_string(0));
		}
	};

	functions["halt"] = [](std::vector<std::string>* bin)
	{
		bin->push_back(std::to_string(OP::HALT));
	};
}