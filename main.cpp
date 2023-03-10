#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

std::map<std::string, unsigned char> opcodes{};
std::map<std::string, unsigned char> compFlags{};
std::map<std::string, unsigned char> regs{};
std::map<std::string, unsigned char> interrupts{};

int main(int argc, char* argv[])
{
	opcodes["MOV"] = 0;
	opcodes["LD"] = 1;
	opcodes["ST"] = 2;
	opcodes["PUSH"] = 3;
	opcodes["POP"] = 4;
	opcodes["IN"] = 5;
	opcodes["OUT"] = 6;
	opcodes["HALT"] = 7;
	opcodes["CMP"] = 8;
	opcodes["JMP"] = 9;
	opcodes["JEQ"] = 10;
	opcodes["JNE"] = 11;
	opcodes["JGT"] = 12;
	opcodes["JGE"] = 13;
	opcodes["JLT"] = 14;
	opcodes["JLE"] = 15;
	opcodes["CALL"] = 16;
	opcodes["RET"] = 17;
	opcodes["INT"] = 18;
	opcodes["ADD"] = 19;
	opcodes["SUB"] = 20;
	opcodes["MUL"] = 21;
	opcodes["DIV"] = 22;
	opcodes["MOD"] = 23;
	opcodes["SHFT"] = 24;
	opcodes["AND"] = 25;
	opcodes["NAND"] = 26;
	opcodes["OR"] = 27;
	opcodes["XOR"] = 28;
	opcodes["NOR"] = 29;
	opcodes["XNOR"] = 30;
	opcodes["NOT"] = 31;

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

	std::map<std::string, unsigned char> labelDefinitions{};
	std::map<std::string, unsigned char> labelReferences{};

	std::vector<char[4]> out{};

	unsigned char pc{};

	for (std::string& line : content)
	{
		std::string instr{ line.substr(0, line.find(' ') + 1) };
		std::string opcode{ line.substr(0, line.find(' ')) };

		if (opcodes.contains(opcode))
		{
			char instruction[4]{};
			instruction[0] |= (0 << 3);
			if (regs.contains(instr.substr(0, instr.find(','))))
				instruction[0] |= regs[instr.substr(0, instr.find(','))];
			else
				instruction[0] |= static_cast<char>(std::stoi(instr.substr(instr.find(','))));
			if (regs.contains(instr.substr(instr.find(' ') + 1)))
				instruction[3] |= regs[instr.substr(instr.find(' ') + 1)];
			else
			{
				instruction[0] |= (1 << 2);
				instruction[1] = static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)) >> 16);
				instruction[2] = static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)) >> 8);
				instruction[3] = static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)));
				out.push_back(instruction);
			}
		}
		else
		{
			labelDefinitions[instr] = 
		}
	}

	return 0;
}