#include <vector>
#include <string>

#include "Assembler.h"
#include "Lightning.h"

std::vector<char> Assembler::assemble(std::vector<std::string>& input)
{
	std::vector<char> bin{};

	short pc{};

	for (const std::string& line : input)
	{
		if (line.empty()) continue;

		std::vector<std::string> words(1);
		for (const char& c : line)
			if (c == '\t') continue;
			else if (c == ' ' && !words.back().empty()) words.push_back("");
			else words.back().push_back(c);

		std::vector<std::string>::iterator opcodeIt{ std::find(Lightning::opcodes.begin(), Lightning::opcodes.end(), words[0]) };
	}

	return bin;
}