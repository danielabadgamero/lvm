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

std::vector<std::string> flags
{
	"TRUE",
	"EQUAL",
	"NOT_EQUAL",
	"LESS",
	"GREATER",
	"ZERO",
	"NOT_ZERO",
	"CARRY"
};

static constexpr bool regChar(const char& c)
{
	return ((c >= 'a' && c <= 'd') || c == 'l' || c == 'h');
}

static unsigned char getRegByte(std::string reg)
{
	unsigned char regByte{ static_cast<unsigned char>(((reg[0] - 'a') << 6) | 0b00'111111) };
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
	std::unordered_map<short, std::string> labelRef{};

	short pc{};

	for (const std::string& line : input)
	{
		if (line.empty()) continue;

		std::vector<std::string> words(1);
		bool quotes{};
		for (const char& c : line)
			if ((c == ' ' || c == '\t') && !words.back().empty() && !quotes) words.push_back("");
			else if (c == '"') quotes ^= 1;
			else if (c != ',' && c != '\t') words.back().push_back(c);

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
				if (words[1] == "wb")
				{
					pc++;
					bin.push_back(static_cast<char>(std::stoi(words[2], NULL, 16)));
				}
				if (words[1] == "wdb")
				{
					pc += 2;
					bin.push_back(static_cast<char>((std::stoi(words[2], NULL, 16) & 0xff00) >> 8));
					bin.push_back(static_cast<char>(std::stoi(words[2], NULL, 16)));
				}
			}
		}
		else
		{
			unsigned char opcode{ static_cast<unsigned char>(std::distance(opcodes.begin(), opcodeIt) << 4) };
			unsigned char dest{};
			unsigned char ddMode{};
			unsigned char src{};
			unsigned char sdMode{};

			unsigned char dReg{};
			unsigned char sReg{};
			short dImm{};
			short sImm{};

			pc++;
			bool reg{ true };
			switch (words.size())
			{
			case 3:
				if (words[1][0] == '[')
					(ddMode = 1 << 2), words[1].erase(words[1].begin()), words[1].pop_back();
				for (const char& c : words[1])
					if (!regChar(c)) reg = false;
				pc++;
				if (!reg)
				{
					dest = 1 << 3;
					pc++;
					if (words[1][0] == '0')
						dImm = static_cast<short>(std::stoi(words[words.size() - 1], NULL, 16));
					else
					{
						std::vector<std::string>::iterator flag{ std::find(flags.begin(), flags.end(), words[1]) };
						if (flag == flags.end())
							labelRef.emplace(static_cast<short>(pc - 2), words[words.size() - 1]);
						else
							dImm = static_cast<short>(std::distance(flags.begin(), flag));
					}
				}
				else
					dReg = getRegByte(words[1]);
			case 2:
				if (words[words.size() - 1][0] == '[')
					(sdMode = 1), words[words.size() - 1].erase(words[words.size() - 1].begin()), words[words.size() - 1].pop_back();
				reg = true;
				for (const char& c : words[words.size() - 1])
					if (!regChar(c)) reg = false;
				pc++;
				if (!reg)
				{
					src = 1 << 1;
					pc++;
					if (words[words.size() - 1][0] == '0')
						sImm = static_cast<short>(std::stoi(words[words.size() - 1], NULL, 16));
					else
						labelRef.emplace(static_cast<short>(pc - 2), words[words.size() - 1]);
				}
				else
					sReg = getRegByte(words[words.size() - 1]);
				break;
			}

			bin.push_back(opcode | dest | ddMode | src | sdMode);
			if (words.size() == 3)
				if (dest == 0)
					bin.push_back(dReg);
				else
				{
					bin.push_back(static_cast<char>((dImm & 0xff00) >> 8));
					bin.push_back(static_cast<char>(dImm));
				}
			if (words.size() == 2 || words.size() == 3)
				if (src == 0)
					bin.push_back(sReg);
				else
				{
					bin.push_back(static_cast<char>((sImm & 0xff00) >> 8));
					bin.push_back(static_cast<char>(sImm));
				}
		}
	}

	for (const auto& label : labelRef)
	{
		bin[label.first] = static_cast<char>((labelDef[label.second] & 0xff00) >> 8);
		bin[label.first + 1ull] = static_cast<char>(labelDef[label.second]);
	}
	
	return bin;
}