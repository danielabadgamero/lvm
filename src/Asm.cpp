#include "Asm.h"
#include "VM.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>

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

static std::vector<unsigned char> compile(const std::filesystem::path& file)
{
	unsigned short offset{ Asm::pc };
	std::vector<unsigned char> out{};

	std::unordered_map<unsigned short, std::string> local_ref{};
	std::unordered_map<std::string, unsigned short> local_def{};

	int lineNum{};
	std::string line{};
	std::ifstream bin{ file.string(), std::ios::binary };
	while (std::getline(bin, line))
	{
		lineNum++;
		if (line.empty()) continue;
		bool code{};
		switch (line[0])
		{
		case '_':
			Asm::global_def.emplace(line.substr(1, line.size() - 2), Asm::pc);
			break;
		case '\t':
			code = true;
			break;
		default:
			local_def.emplace(line.substr(0, line.size() - 1), Asm::pc);
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
					case '0': words.back().push_back('\0'); break;
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
				Asm::pc++;
				out.push_back(static_cast<unsigned char>(parseBytes(words[1])));
				continue;
			}
			else if (words[0] == "ws")
			{
				for (const char& c : words[1]) out.push_back(c);
				Asm::pc += words[1].size();
				continue;
			}
			else if (words[0] == "rb")
			{
				unsigned short num{ parseBytes(words[1]) };
				Asm::pc += num;
				for (unsigned short i{}; i != num; i++) out.push_back(0);
				continue;
			}

			std::string suffix{};
			if (words[0].size() > 3) suffix = words[0].substr(3);
			unsigned char suffix_bits{};
			if (!suffix.empty()) suffix_bits = static_cast<unsigned char>(std::stoi(suffix, nullptr, 2));

			if (!VM::opcodes.contains(words[0].substr(0, 3)))
			{
				std::cout << "Unknown opcode: \"" << words[0].substr(0, 3) << "\" (" << lineNum << ", " << file.filename() << ')' << std::endl;
				return {};
			}

			unsigned char opcode{ VM::opcodes.at(words[0].substr(0, 3)) };
			unsigned char opByte{ static_cast<unsigned char>(opcode << 4) };
			if (!suffix.empty()) opByte |= suffix_bits;

			Asm::pc++;
			if (words.size() == 1)
			{
				out.push_back(opByte);
				continue;
			}

			// bit	desc
			// 0	srce and dest: 1 byte (0) / 2 bytes (1)
			// 1-2	srce: imm(0) / RAM(1) / RAM[RAM](2)
			// 3	dest: RAM(0) / RAM[RAM](1)
			// 4-7	opcode

			unsigned short dest{};
			switch (words[1][0])
			{
			case '@':
				switch (words[1][1])
				{
				case '#': dest = parseBytes(words[1].substr(2)); break;
				case '_': suffix_bits |= 0b1000; Asm::global_ref.emplace(Asm::pc, words[1].substr(2)); break;
				default: suffix_bits |= 0b1000; local_ref.emplace(Asm::pc, words[1].substr(1));
				} break;
			case '#': dest = parseBytes(words[1].substr(1)); break;
			case '_': Asm::global_ref.emplace(Asm::pc, words[1].substr(1)); break;
			default: local_ref.emplace(Asm::pc, words[1]);
			}
			Asm::pc += 2;

			opByte |= suffix_bits;
			if (words.size() == 2)
			{
				out.push_back(opByte);
				out.push_back((dest & 0xff00) >> 8);
				out.push_back(dest & 0xff);
				continue;
			}

			unsigned short srce{};
			switch (words[2][0])
			{
			case '#': srce = parseBytes(words[2].substr(1)); break;
			case '@':
				switch (words[2][1])
				{
				case '@':
					if (words[2][2] == '_') { suffix_bits |= 0b0100; Asm::global_ref.emplace(Asm::pc, words[2].substr(3)); }
					else { suffix_bits |= 0b0100; local_ref.emplace(Asm::pc, words[2].substr(2)); }
					break;
				case '#': suffix_bits |= 0b0010; srce = parseBytes(words[2].substr(2)); break;
				case '_': suffix_bits |= 0b0010; Asm::global_ref.emplace(Asm::pc, words[2].substr(2)); break;
				default: suffix_bits |= 0b0010; local_ref.emplace(Asm::pc, words[2].substr(1));
				} break;
			case '_': Asm::global_ref.emplace(Asm::pc, words[2].substr(1)); break;
			default: local_ref.emplace(Asm::pc, words[2]);
			}
			Asm::pc += 2;

			opByte |= suffix_bits;
			out.push_back(opByte);
			out.push_back((dest & 0xff00) >> 8);
			out.push_back(dest & 0xff);
			out.push_back((srce & 0xff00) >> 8);
			out.push_back(srce & 0xff);
		}
	}

	for (const std::pair<const unsigned short, std::string>& ref : local_ref)
	{
		if (!local_def.contains(ref.second))
		{
			std::cout << "Undefined symbol: \"" << ref.second << "\" (" << ref.first << ", " << file.filename() << ')' << std::endl;
			return {};
		}
		unsigned short val{ local_def.at(ref.second) };
		out.at(ref.first - offset) = (val & 0xff00) >> 8;
		out.at(ref.first + 1 - offset) = val & 0xff;
	}

	return out;
}

void Asm::assemble(const std::filesystem::path& prog_path)
{
	pc = 0;
	global_ref.clear();
	global_def.clear();
	std::unordered_set<std::string> files{};

	for (const std::filesystem::directory_entry& d : std::filesystem::directory_iterator(prog_path))
		if (d.path().extension() == ".asm") files.emplace(d.path().string());
	for (const std::filesystem::directory_entry& d : std::filesystem::directory_iterator("./prog/llib"))
		if (d.path().extension() == ".asm") files.emplace(d.path().string());

	std::vector<unsigned char> bin{};

	for (const std::string& file : files)
	{
		std::vector<unsigned char> out{ compile(std::filesystem::path{ file }) };
		if (out.empty())
		{
			std::cout << "Can't continue due to " << file << std::endl;
			return;
		}
		for (const unsigned char& c : out) bin.push_back(c);
	}

	for (const std::pair<const unsigned short, std::string>& ref : global_ref)
	{
		if (!global_def.contains(ref.second))
		{
			std::cout << "Undefined symbol: \"" << ref.second << "\" (" << ref.first << ')' << std::endl;
			return;
		}
		unsigned short val{ global_def.at(ref.second) };
		bin.at(ref.first) = (val & 0xff00) >> 8;
		bin.at(ref.first + 1) = val & 0xff;
	}

	std::ofstream out{ (prog_path / "bin").string(), std::ios::binary | std::ios::trunc };
	std::ofstream sym{ (prog_path / "sym").string(), std::ios::binary | std::ios::trunc };

	out.write((char*)bin.data(), bin.size());

	if (!global_def.contains("main"))
	{
		std::cout << "Enty point not defined (did you define \"_main\"?)" << std::endl;
		return;
	}
	sym.write((char*)(&global_def.at("main")), 2);
}
