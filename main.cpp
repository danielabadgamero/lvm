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
	// opcodes[""] = 1;
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

	path = "test.lasm";
	
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
		else if (c != '\t')
			content.back().push_back(c);
	}

	std::vector<Label> labelDefinitions{};
	std::vector<Label> labelReferences{};

	std::vector<char> out{};

	int pc{};

	// Loop through each line
	for (std::string& line : content)
	{
		std::string opcode{ line.substr(0, line.find(' ')) };
		std::string operands{ line.substr(line.find(' ') + 1) };
		std::string dest{ operands.substr(0, operands.find(',')) };
		std::string source{ operands.substr(operands.find(' ') + 1) };

		// Opcode instruction
		if (opcodes.contains(opcode))
		{
			out.push_back(opcodes[opcode] << 3);

			// rDest
			if (regs.contains(dest))
				out.back() |= regs[dest];
			else
				out.back() |= static_cast<char>(std::stoi(dest, nullptr, 16));

			// rSource / imm
			if (regs.contains(source))
			{
				out.push_back(0);
				out.push_back(0);
				out.push_back(regs[source]);
			}
			else
			{
				out.back() |= (1 << 2);
				try
				{
					out.push_back(static_cast<char>(std::stoi(source, nullptr, 16) >> 16));
					out.push_back(static_cast<char>(std::stoi(source, nullptr, 16) >> 8));
					out.push_back(static_cast<char>(std::stoi(source, nullptr, 16)));
				}
				catch (...)
				{
					if (interrupts.contains(source))
					{
						out.push_back(0);
						out.push_back(0);
						out.push_back(interrupts[source]);
					}
					else if (source == "$")
					{
						out.push_back(static_cast<char>(pc << 16));
						out.push_back(static_cast<char>(pc << 8));
						out.push_back(static_cast<char>(pc));
					}
					else
					{
						labelReferences.push_back({ source, pc + 1 });
						out.push_back(0);
						out.push_back(0);
						out.push_back(0);
					}
				}
			}
			pc += 4;
		}
		// Label
		else if (!line.empty())
		{
			labelDefinitions.push_back({ opcode.substr(0, opcode.find(':')), pc });
			// Label for data
			if (line.find(':') != line.size() - 1)
				if (operands[0] == '"')
					for (int i{ 1 }; i < operands.size() - 2; i++)
						if (operands[i] == '\\')
						{
							i++;
							switch (operands[i])
							{
							case '0':
								out.push_back('\0');
								break;
							case 'n':
								out.push_back('\n');
								break;
							case 't':
								out.push_back('\t');
								break;
							case '\\':
								out.push_back('\\');
								break;
							default:
								std::cout << "Unrecognized escape sequence at line \"" << line << "\".";
								goto end;
							}
							i++;
							pc++;
						}
						else
						{
							out.push_back(operands[i]);
							pc++;
						}
				else
				{
					out.push_back(static_cast<char>(std::stoi(operands, nullptr, 16) >> 16));
					out.push_back(static_cast<char>(std::stoi(operands, nullptr, 16) >> 8));
					out.push_back(static_cast<char>(std::stoi(operands, nullptr, 16)));
					pc += 3;
				}
		}
	}

	// Resolve labels
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

	end:
	return 0;
}