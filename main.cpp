#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "Opcodes.h"
#include "Regs.h"
#include "Keywords.h"

struct Label
{
	std::string id{};
	int addr{};
};

std::vector<std::string> getContent(std::ifstream& input)
{
	std::vector<std::string> content(1);
	while (!input.eof())
	{
		char c{};
		input.read(&c, 1);
		if (c == '\n')
			content.push_back("");
		else if (c == '\t')
			if (!content.back().empty())
				content.back().push_back(' ');
		else
			content.back().push_back(c);
	}
	return content;
}

int main(int argc, char* argv[])
{
	std::string path{};
	if (argc == 2)
		path = argv[1];
	else
		std::cerr << "No input file.\n";
	
	std::ifstream input{ path };
	std::string fileName{ path.substr(path.find_last_of('\\') + 1).substr(0, path.find('.')) };
	std::ofstream output{ fileName + ".bin" };

	std::vector<std::string> content{ getContent(input) };

	std::vector<char> out{};
	std::vector<Label> labelDefinitions{};
	std::vector<Label> labelReferences{};

	int pc{};

	for (std::string& line : content)
	{
		if (line.empty())
			continue;

		std::vector<std::string> args(1);
		for (char c : line)
			switch (c)
			{
			case ' ':
				args.push_back("");
			case ',':
			case ':':
				break;
			default:
				args.back().push_back(c);
			}
		
		if (std::find(keywords.begin(), keywords.end(), args[0]) != keywords.end())
		{
			// Keyword
		}
		else if (std::find(opcodes.begin(), opcodes.end(), args[0]) != opcodes.end())
		{
			// Instruction
		}
		else
		{
			// Label
		}
	}

	return 0;
}