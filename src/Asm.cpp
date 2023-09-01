#include "Asm.h"
#include "VM.h"

#include <vector>
#include <fstream>
#include <iostream>

static unsigned short parseBytes(const std::string& str)
{
	switch (str[0])
	{
	case '0':
		if (str.size() == 1) return 0;
		switch (str[1])
		{
		case 'x': return static_cast<unsigned short>(std::stoi(str.substr(2), nullptr, 16));
		case 'b': return static_cast<unsigned short>(std::stoi(str.substr(2), nullptr, 2));
		default: return static_cast<unsigned short>(std::stoi(str.substr(1), nullptr, 8));
		}
	case '\'': return str[1];
	default: return std::stoi(str);
	}
}

void Asm::assemble(const std::filesystem::path& prog_path)
{
	std::string prog_name{ prog_path.filename() };
	std::filesystem::path asm_path{ prog_path / (prog_name + ".asm") };
	if (!std::filesystem::exists(asm_path))
	{
		std::cout << "Assembly file not found" << std::endl;
		return;
	}
	std::ifstream assembly{ asm_path.string() };
	
	std::vector<unsigned char> out{};
	unsigned short pc{};
	std::string line{};
	int lineNum{};
	while (std::getline(assembly, line))
	{
		lineNum++;
		if (line.empty()) continue;
		bool code{};
		switch (line[0])
		{
		case '.':
			command_def.emplace(line.substr(1, line.size() - 2), pc);
			break;
		case '\t':
			code = true;
			break;
		default:
			sym_def.emplace(line.substr(0, line.size() - 1), pc);
		}

		if (code)
		{
			std::vector<std::string> words(1);
			for (std::string::iterator c{ line.begin() }; c != line.end(); c++)
			{
				if (*c == '\\')
					switch (*(++c))
					{
					case 'n': words.back().push_back('\n'); break;
					case 't': words.back().push_back('\t'); break;
					case 'b': words.back().push_back('\b'); break;
					default: words.back().push_back(*c); break;
					}
				else if ((*c == ' ' || *c == '\t') && !words.back().empty()) words.push_back("");
				else if (*c == ';')
				{
					if (words.back().empty()) words.pop_back();
					break;
				}
				else if (*c != ' ' && *c != '\t') words.back().push_back(*c);
			}

			if (words[0] == "wb")
			{
				pc++;
				out.push_back(static_cast<unsigned char>(parseBytes(words[1])));
				continue;
			}

			std::string suffix{};
			if (words[0].size() > 3) suffix = words[0].substr(3);
			unsigned char suffix_bits{};
			if (!suffix.empty()) suffix_bits = static_cast<unsigned char>(std::stoi(suffix, nullptr, 2));

			unsigned char opcode{ VM::opcodes.at(words[0].substr(0, 3)) };
			unsigned char opByte{ static_cast<unsigned char>(opcode << 4) };
			if (!suffix.empty()) opByte |= suffix_bits;

			pc++;
			if (words.size() == 1)
			{
				out.push_back(opByte);
				continue;
			}

			// bit  desc
			// 0-1  srce: imm(0) / RAM[stack](1) / RAM(2) / stack(3)
			// 2-3  dest: RAM(0) / RAM[stack](1) / RAM[RAM](2) / stack(3)
			// 4-7  opcode

			unsigned short dest{};
			bool hasDest{};
			switch (words[1][0])
			{
			case '@':
				switch (words[1][1])
				{
				case '@': if (suffix.empty()) { suffix_bits = 0b1000; } sym_ref.emplace(pc, words[1].substr(2)); pc += 2; hasDest = true; break;
				case '^': if (suffix.empty()) { suffix_bits = 0b0100; } break;
				default: sym_ref.emplace(pc, words[1].substr(1)); pc += 2; hasDest = true;
				} break;
			case '^': if (suffix.empty()) suffix_bits = 0b1100; break;
			case '#': hasDest = true; pc += 2; dest = parseBytes(words[1].substr(1)); break;
			default: sym_ref.emplace(pc, words[1]); pc += 2; hasDest = true;
			}

			if (suffix.empty()) opByte |= suffix_bits;
			if (words.size() == 2)
			{
				out.push_back(opByte);
				if (hasDest)
				{
					out.push_back((dest & 0xff00) >> 8);
					out.push_back(dest & 0xff);
				}
				continue;
			}

			unsigned short srce{};
			bool hasSrce{};
			switch (words[2][0])
			{
			case '#': srce = parseBytes(words[2].substr(1)); hasSrce = true; pc += 2; break;
			case '@':
				switch (words[2][1])
				{
				case '^': if (suffix.empty()) { suffix_bits |= 0b0001; } break;
				case '#': if (suffix.empty()) { suffix_bits |= 0b0010; } hasSrce = true; pc += 2; srce = parseBytes(words[2].substr(2)); break;
				default: if (suffix.empty()) { suffix_bits |= 0b0010; } hasSrce = true; sym_ref.emplace(pc, words[2].substr(1)); pc += 2;
				} break;
			case '^': if (suffix.empty()) { suffix_bits |= 0b0011; } break;
			default: hasSrce = true; sym_ref.emplace(pc, words[2]); pc += 2;
			}
			if (suffix.empty()) opByte |= suffix_bits;
			out.push_back(opByte);
			if (hasDest)
			{
				out.push_back((dest & 0xff00) >> 8);
				out.push_back(dest & 0xff);
			}
			if (hasSrce)
			{
				out.push_back((srce & 0xff00) >> 8);
				out.push_back(srce & 0xff);
			}
		}
	}

	for (const auto& ref : sym_ref)
	{
		unsigned short val{ static_cast<unsigned short>(sym_def.at(ref.second) + static_cast<unsigned short>(1024)) };
		out.at(ref.first) = (val & 0xff00) >> 8;
		out.at(ref.first + 1) = val & 0xff;
	}

	std::cout << "Symbol definitions:" << std::hex << std::endl;
	for (const auto& s : sym_def)
		std::cout << s.first << ' ' << s.second << std::endl;
	std::cout << "\nSymbol references:" << std::endl;
	for (const auto& s : sym_ref)
		std::cout << s.first << ' ' << s.second << std::endl;
	std::cout << "\nCommand definitions:" << std::endl;
	for (const auto& c : command_def)
		std::cout << c.first << ' ' << c.second << std::endl;

	std::ofstream bin{ (prog_path / (prog_name + ".bin")).string(), std::ios::binary | std::ios::trunc };
	std::ofstream sym{ (prog_path / (prog_name + ".sym")).string(), std::ios::binary | std::ios::trunc };

	bin.write((char*)out.data(), out.size());
	for (auto& c : command_def)
	{
		c.second += 1024;
		sym.write(c.first.c_str(), c.first.size());
		char sep[1]{ ':' };
		sym.write(sep, 1);
		sym.write((char*)(&c.second), 2);
	}
}
