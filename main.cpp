#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

typedef void(*opcodeFunc)(char**, std::string);

std::map<std::string, opcodeFunc> opcodes{};
std::map<std::string, unsigned char> compFlags{};
std::map<std::string, unsigned char> regs{};
std::map<std::string, unsigned char> interrupts{};

int main(int argc, char* argv[])
{
	compFlags["equal"] = 0;
	compFlags["not_equal"] = 1;
	compFlags["greater"] = 2;
	compFlags["greater_equal"] = 3;
	compFlags["less"] = 4;
	compFlags["less_equal"] = 5;

	regs["ax"] = 0;
	regs["bx"] = 1;
	regs["cx"] = 2;
	regs["dx"] = 3;

	interrupts["print_char"] = 0;
	interrupts["new_line"] = 1;
	interrupts["read_disk"] = 2;
	interrupts["write_disk"] = 3;

	opcodes["MOV"] = [](char** dest, std::string instr)
	{
		*dest[0] |= (0 << 3);
		*dest[0] |= regs[instr.substr(0, instr.find(','))];
		if (regs.contains(instr.substr(instr.find(' ') + 1)))
			*dest[3] |= regs[instr.substr(instr.find(' ') + 1)];
		else
		{
			*dest[0] |= (1 << 2);
			*dest[1] = static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)) >> 16);
			*dest[2] = static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)) >> 8);
			*dest[3] = static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)));
		}
	};

	std::string path{};

	if (argc == 2)
		for (int i{}; argv[1][i] != '\0'; i++)
			path.push_back(argv[1][i]);

	std::ifstream input{ path };
	std::string fileName{ path.substr(path.find_last_of('\\') + 1).substr(0, path.find('.')) };
	std::ofstream output{ fileName + ".bin" };

	std::vector<std::string> content(1);

	while (!input.eof())
	{
		char c{};
		input.read(&c, 1);
		if (c == '\n')
			content.push_back("");
		else
			content.back().push_back(c);
	}

	for (std::string& instr : content)
	{
		std::string opcode{ instr.substr(0, instr.find(' ')) };
		if (opcodes.contains(opcode))
		{
			char* instruction{ new char[4] {} };
			opcodes[opcode](&instruction, instr.substr(instr.find(' ') + 1));
			output.write(instruction, 4);
		}
		else
		{
			// keywords
		}
	}

	return 0;
}