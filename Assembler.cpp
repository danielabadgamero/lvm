#include <vector>
#include <string>
#include <unordered_map>

#include "Assembler.h"
#include "Lightning.h"

std::vector<char> Assembler::assemble(std::vector<std::string>& input)
{
	std::vector<char> bin{};
	std::unordered_map<std::string, short> labelDef{};
	std::unordered_map<std::string, short> labelRef{};

	short pc{};

	for (const std::string& line : input)
	{
		if (line.empty()) continue;

		std::vector<std::string> words(1);
		bool quotes{};
		for (const char& c : line)
			if (c == '\t') continue;
			else if (c == ' ' && !words.back().empty() && !quotes) words.push_back("");
			else if (c == '"') quotes ^= 1;
			else words.back().push_back(c);

		std::vector<std::string>::iterator opcodeIt{ std::find(Lightning::opcodes.begin(), Lightning::opcodes.end(), words[0]) };
		if (opcodeIt == Lightning::opcodes.end())
		{
			if (words.size() > 1)
			{
				if (words[1] == "ws")
				{
					bool escaped{};
					for (const char& c : words[2]);
				}
			}
			else
			{

			}
		}
		else
		{
			unsigned char opcode{ static_cast<unsigned char>(std::distance(Lightning::opcodes.begin(), opcodeIt)) };
		}
	}
	
	return bin;
}