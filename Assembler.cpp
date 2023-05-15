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

static constexpr bool regChar(const char& c)
{
	return ((c >= 'a' && c <= 'd') || c == 'l' || c == 'h');
}

static unsigned char getRegByte(std::string reg)
{
	unsigned char regByte{ ((reg[0] - 'a') << 6) | 0b00'111111 };
	for (int i{ 1 }; i != reg.size(); i++)
		if (reg[i] == 'h')
			regByte ^= (0b01'00'00'00 >> (2 * i));
		else regByte ^= (0b10'00'00'00 >> (2 * i));

	return regByte;
}

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
		if (opcodeIt == opcodes.end())
		{
			words[0].pop_back();
			labelDef.emplace(words[0], pc);
			if (words.size() > 1)
			{
				if (words[1] == "ws")
				{
					bool escaped{};
					for (const char& c : words[2])
					{
						if (escaped)
						{
							switch (c)
							{
							case '0': bin.push_back('\0'); break;
							case '\\': bin.push_back('\\'); break;
							case 'n': bin.push_back('\n'); break;
							case 't': bin.push_back('\t'); break;
							case 'b': bin.push_back('\b'); break;
							}
							escaped = false;
							pc++;
							continue;
						}
						if (c == '\\')
							escaped = true;
						else
							bin.push_back(c), pc++;
					}
				}
			}
		}
		else
		{
			unsigned char opcode{ static_cast<unsigned char>(std::distance(opcodes.begin(), opcodeIt)) << 4 };
			unsigned char dest{};
			unsigned char ddMode{};
			unsigned char src{};
			unsigned char sdMode{};

			switch (words.size())
			{
			case 3:
				if (words[1][0] == '[')
					(ddMode = 1 << 2), words[1].erase(words[1].begin()), words[1].pop_back();
				bool reg{ true };
				for (const char& c : words[1])
					if (!regChar(c)) reg = false;
				if (!reg)
				{
					dest = 1 << 3;
					try
					{
						short n{ std::stoi(words[1], NULL, 16) };
						bin.push_back((n & 0xff00) >> 8);
						bin.push_back(static_cast<char>(n));
						pc += 2;
					}
					catch (...)
					{
						labelRef.emplace(words[1], pc);
						bin.push_back(0);
						bin.push_back(0);
						pc += 2;
					}
				}
				else
					bin.push_back(getRegByte(words[1]));
			case 2:
				if (words[words.size() - 1][0] == '[')
					(sdMode = 1), words[1].erase(words[words.size() - 1].begin()), words[words.size() - 1].pop_back();
				reg = true;
				for (const char& c : words[words.size() - 1])
					if (!regChar(c)) reg = false;
				if (!reg)
				{
					src = 1 << 1;
				}
				else
					bin.push_back(getRegByte(words[words.size() - 1]));
				break;
			}
		}
	}
	
	return bin;
}