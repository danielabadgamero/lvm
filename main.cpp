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

struct Instruction
{
	unsigned char aMode : 1;	//	defines interpretation of immediate (if existent)
	unsigned char dAddr : 3;	//	destination address
	unsigned char opcode : 4;	//	16 different opcodes

	unsigned char getInstruction()
	{
		return (opcode << 4) | (dAddr << 1) | aMode;
	}

	Instruction& operator=(unsigned char data)
	{
		opcode = (data & 0b1111'0000) >> 4;
		dAddr = (data & 0b0000'1110) >> 1;
		aMode = (data & 0b0000'0001);

		return *this;
	}
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
		{
			if (!content.back().empty())
				content.back().push_back(' ');
		}
		else if (c != '\0')
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
	std::ofstream output{ fileName + ".bin", std::ostream::binary };

	std::vector<std::string> content{ getContent(input) };

	std::vector<unsigned char> out{};
	std::vector<Label> labelDefinitions{};
	std::vector<Label> labelReferences{};

	int pc{};

	for (std::string& line : content)
	{
		if (line.empty())
			continue;

		std::vector<std::string> args(1);
		bool string{};
		for (char c : line)
			switch (c)
			{
			case ' ':
				if (!string)
					args.push_back("");
			case ',':
			case ':':
				if (string)
					args.back().push_back(c);
				break;
			case '"':
				string = !string;
				break;
			default:
				args.back().push_back(c);
			}

		std::vector<std::string>::iterator keyword{ std::find(keywords.begin(), keywords.end(), args[0]) };
		std::vector<std::string>::iterator opcode{ std::find(opcodes.begin(), opcodes.end(), args[0]) };
		
		if (keyword != keywords.end())
		{
			int num{ std::stoi(args[1].substr(2), nullptr, 16) };
			switch (std::distance(keywords.begin(), keyword))
			{
			case wb:
				pc++;
				out.push_back(static_cast<unsigned char>(num));
				break;
			case wd:
				pc += 2;
				out.push_back(static_cast<unsigned char>(num >> 8));
				out.push_back(static_cast<unsigned char>(num));
				break;
			case wq:
				pc += 4;
				out.push_back(static_cast<unsigned char>(num >> 24));
				out.push_back(static_cast<unsigned char>(num >> 16));
				out.push_back(static_cast<unsigned char>(num >> 8));
				out.push_back(static_cast<unsigned char>(num));
				break;
			case ws:
				for (const char& c : args[1])
					out.push_back(c);
				pc += static_cast<int>(args[1].size());
				out.push_back('\0');
			}
		}
		else if (opcode != opcodes.end())
		{
			Instruction instruction{};
			instruction.opcode = std::distance(opcodes.begin(), opcode);
			instruction.dAddr = std::distance(regs.begin(), std::find(regs.begin(), regs.end(), args[1]));
			switch (instruction.opcode)
			{
			case SWTCH:
			case POP:
			case NOT:
				out.push_back(instruction.getInstruction());
				out.push_back(0);
				break;
			default:
				if (std::find(regs.begin(), regs.end(), args[2]) != regs.end())
				{
					out.push_back(instruction.getInstruction());
					out.push_back(static_cast<unsigned char>(std::distance(regs.begin(), std::find(regs.begin(), regs.end(), args[2]))));
				}
				else try
				{
					int num{ std::stoi(args[2].substr(2), nullptr, 16)};
					instruction.aMode = 1;
					out.push_back(instruction.getInstruction());
					out.push_back(static_cast<unsigned char>(num >> 16));
					out.push_back(static_cast<unsigned char>(num >> 8));
					out.push_back(static_cast<unsigned char>(num));
					pc += 2;
				}
				catch (std::invalid_argument)
				{
					instruction.aMode = 1;
					out.push_back(instruction.getInstruction());
					if (args[2][0] == '\'')
					{
						char c{};
						if (args[2][1] == '\\')
							switch (args[2][2])
							{
							case '0': c = '\0'; break;
							case 'a': c = '\a'; break;
							case 'b': c = '\b'; break;
							case 't': c = '\t'; break;
							case 'n': c = '\n';	break;
							case 'v': c = '\v'; break;
							case 'f': c = '\f'; break;
							case 'r': c = '\r'; break;
							default:
								c = args[2][2];
							}
						else
							c = args[2][1];
						out.push_back('\0');
						out.push_back('\0');
						out.push_back(c);
					}
					else
					{
						labelReferences.push_back({ args[2], pc + 1 });
						out.push_back('\0');
						out.push_back('\0');
						out.push_back('\0');
					}
					pc += 2;
				}
			}
			pc += 2;
		}
		else
			labelDefinitions.push_back({ args[0], pc });
	}

	for (Label& labelRef : labelReferences)
	{
		std::vector<Label>::iterator labelDef{ labelDefinitions.end() };
		for (std::vector<Label>::iterator label{ labelDefinitions.begin() }; label != labelDefinitions.end(); label++)
			if (label->id == labelRef.id)
			{
				labelDef = label;
				break;
			}

		if (labelDef == labelDefinitions.end())
			std::cerr << "Undefined label reference: " << labelRef.id << " at pc " << labelRef.addr << '\n';

		out[labelRef.addr] = static_cast<unsigned char>(labelDef->addr >> 16);
		out[labelRef.addr + 1] = static_cast<unsigned char>(labelDef->addr >> 8);
		out[labelRef.addr + 2] = static_cast<unsigned char>(labelDef->addr);
	}

	output.write((const char*)out.data(), out.size());

	return 0;
}