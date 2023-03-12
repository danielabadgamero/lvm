#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

std::map<std::string, char> opcodes{};
std::map<std::string, char> compFlags{};
std::map<std::string, char> regs{};
std::map<std::string, char> interrupts{};

struct Label
{
	std::string id{};
	int addr{};
};

int main(int argc, char* argv[])
{
	opcodes["MOV"] = 0;
	opcodes["SPB"] = 1;
	opcodes["LD"] = 2;
	opcodes["ST"] = 3;
	opcodes["PUSH"] = 4;
	opcodes["POP"] = 5;
	opcodes["IN"] = 6;
	opcodes["OUT"] = 7;
	opcodes["HALT"] = 8;
	opcodes["CMP"] = 9;
	opcodes["JMP"] = 10;
	opcodes["JEQ"] = 11;
	opcodes["JNE"] = 12;
	opcodes["JGT"] = 13;
	opcodes["JGE"] = 14;
	opcodes["JLT"] = 15;
	opcodes["JLE"] = 16;
	opcodes["CALL"] = 17;
	opcodes["RET"] = 18;
	opcodes["INT"] = 19;
	opcodes["ADD"] = 20;
	opcodes["SUB"] = 21;
	opcodes["MUL"] = 22;
	opcodes["DIV"] = 23;
	opcodes["MOD"] = 24;
	opcodes["SHFT"] = 25;
	opcodes["AND"] = 26;
	opcodes["NAND"] = 27;
	opcodes["OR"] = 28;
	opcodes["XOR"] = 29;
	opcodes["NOR"] = 30;
	opcodes["NOT"] = 31;

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

	std::vector<Label> labelDefinitions{};
	std::vector<Label> labelReferences{};

	std::vector<char> out{};

	char pc{};

	for (std::string& line : content)
	{
		std::string instr{ line.substr(line.find(' ') + 1) };
		std::string opcode{ line.substr(0, line.find(' ')) };

		if (opcodes.contains(opcode))
		{
			out.push_back(opcodes[opcode] << 3);
			if (regs.contains(instr.substr(0, instr.find(','))))
				out.back() |= regs[instr.substr(0, instr.find(','))];
			else
				out.back() |= static_cast<char>(std::stoi(instr.substr(0, instr.find(','))));
			if (regs.contains(instr.substr(instr.find(' ') + 1)))
			{
				out.push_back(0);
				out.push_back(0);
				out.push_back(regs[instr.substr(instr.find(' ') + 1)]);
			}
			else
			{
				out.back() |= (1 << 2);
				try
				{
					out.push_back(static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)) >> 16));
					out.push_back(static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1)) >> 8));
					out.push_back(static_cast<char>(std::stoi(instr.substr(instr.find(' ') + 1))));
				}
				catch (...)
				{
					if (interrupts.contains(instr.substr(instr.find(' ') + 1)))
					{
						out.push_back(0);
						out.push_back(0);
						out.push_back(interrupts[instr.substr(instr.find(' ') + 1)]);
					}
					else
					{
						labelReferences.push_back({ instr.substr(instr.find(' ') + 1), pc + 1 });
						out.push_back(0);
						out.push_back(0);
						out.push_back(0);
					}
				}
			}
			pc += 4;
		}
		else if (!instr.empty())
		{
			labelDefinitions.push_back({ opcode.substr(0, opcode.find(':')), pc });
			if (instr.find(':') != instr.size() - 1)
				if (instr[0] == '"')
				{
					for (int i{ 1 }; i != instr.size() - 2; i++)
					{
						out.push_back(instr[i]);
						pc++;
					}
					out.push_back('\0');
					pc++;
				}
				else
				{
					out.push_back(static_cast<char>(std::stoi(instr)));
					pc++;
				}
		}
	}

	for (std::vector<Label>::iterator i{ labelReferences.begin() }; i != labelReferences.end(); i++)
	{
		Label targetLabel{};
		for (std::vector<Label>::iterator j{ labelDefinitions.begin() }; j != labelDefinitions.end(); j++)
			if (j->id == i->id)
				targetLabel = *j;
		out.at(i->addr) = static_cast<char>(targetLabel.addr << 16);
		out.at(static_cast<size_t>(i->addr + 1)) = static_cast<char>(targetLabel.addr << 8);
		out.at(static_cast<size_t>(i->addr + 2)) = static_cast<char>(targetLabel.addr);
	}

	for (char c : out)
		output.write(&c, 1);

	return 0;
}