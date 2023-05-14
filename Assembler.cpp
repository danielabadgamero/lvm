#include <vector>
#include <string>
#include <unordered_map>

#include "Assembler.h"

std::vector<std::string> opcodes
{
	// Data management
	"MOV",
	"PUSH",
	"POP",
	"RD",
	"WR",
	"REC",

	// Flow control
	"HALT",
	"JMP",
	"CALL",
	"RET",

	// Arithmetic and bitwise logic
	"ADD",
	"MUL",
	"DIV",
	"CMP",
	"AND",
	"NOT"
};

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
			if ((c == ' ' || c == '\t') && !words.back().empty() && !quotes) words.push_back("");
			else if (c == '"') quotes ^= 1;
			else if (c != ',') words.back().push_back(c);

		std::vector<std::string>::iterator opcodeIt{ std::find(opcodes.begin(), opcodes.end(), words[0]) };
	}
	
	return bin;
}