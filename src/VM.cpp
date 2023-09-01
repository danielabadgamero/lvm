#include "VM.h"
#include "Core.h"
#include "Dev.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#define OPCODE ((opByte & 0xf0) >> 4)
#define DEST(v) { if (((opByte & 0b1100) >> 2) == 3) { stack.top() = v; } else { RAM[dest] = (v); } }

static unsigned short nextShort()
{
	unsigned short next{ static_cast<unsigned short>((VM::RAM[VM::pc] << 8) | VM::RAM[VM::pc + 1]) };
	VM::pc += 2;
	return next;
}

void VM::loadCommands(const std::filesystem::path& path)
{
	prog_path = path;
	prog_name = prog_path.filename();
	std::filesystem::path sym_path{ prog_path / (prog_name + ".sym") };
	std::ifstream sym{ sym_path.string(), std::ios::binary };

	char byte{};
	std::string command{};
	while (!sym.eof())
	{
		sym.read(&byte, 1);
		if (sym.eof()) break;
		if (byte == ':')
		{
			unsigned short address{};
			sym.read((char*)(&address), 2);
			commands[command] = address;
			command.clear();
		}
		else command.push_back(byte);
	}

	std::filesystem::path bin_path{ prog_path / (prog_name + ".bin") };
	std::ifstream bin{ bin_path.string(), std::ios::binary };
	size_t size{ std::filesystem::file_size(bin_path) };
	if (size > ((1 << 16) - 1024))
	{
		std::cout << "Programme is too big" << std::endl;
		return;
	}

	bin.read((char*)RAM + 1024, size);

	if (std::filesystem::exists(prog_path / "disk"))
	{
		std::ifstream disk_out{ (prog_path / "disk").string(), std::ios::binary };
		disk_out.read((char*)disk, 1 << 16);
	}

	Core::executing = true;
}

void VM::execute(const std::string& command)
{
	pc = commands[command];
	running = true;

	while (running)
	{
		unsigned char opByte{ RAM[pc] };
		flags[always_true] = true;
		pc++;

		// std::cout << std::hex << std::find_if(opcodes.begin(), opcodes.end(), [&](const std::pair<std::string, unsigned char>& p){ return p.second == OPCODE; })->first << std::endl;

		switch (OPCODE)
		{
		case HLT:
			if ((opByte & 1) == 0)
			{
				Core::executing = false;
				for (unsigned char& b : RAM) b = 0;
				std::ofstream disk_out{ (prog_path / "disk").string(), std::ios::binary };
				disk_out.write((char*)disk, sizeof(disk));
				prog_path = "";
				prog_name.clear();
				return;
			}
			else running = false;
			continue;
		case PSP: if (opByte & 1) stack.push(0); else stack.pop(); continue;
		}

		// bit	desc
		// 0-1	srce: imm(0) / RAM[stack](1) / RAM(2) / stack(3)
		// 2-3	dest: RAM(0) / RAM[stack](1) / RAM[RAM](2) / stack(3)
		// 4-7	opcode

		unsigned short dest{};
		if (((opByte & 4) >> 2) == 0 || OPCODE == JMP || OPCODE == SIG) dest = nextShort();
		else dest = stack.top();
		if (((opByte & 12) >> 2) == 2) dest = static_cast<unsigned short>(RAM[dest] << 8) | RAM[dest + 1];
		unsigned short DEST_VAL{};
		if (((opByte & 12) >> 2) == 3) DEST_VAL = stack.top();
		else DEST_VAL = RAM[dest]; 
		switch (OPCODE)
		{
		case SIG: Dev::devices[(dest & 0xff00) >> 8](dest & 0xff); continue;
		case CLR: if (opByte & 1) { stack.push(pc); pc = dest; } else { pc = stack.top(); stack.pop(); } continue;
		case JMP: if (flags[opByte & 0xf]) pc = dest; continue;
		case NOT: DEST(~DEST_VAL) continue;
		}

		unsigned short srce{};
		if ((opByte & 1) == 0) srce = nextShort();
		switch (opByte & 3)
		{
		case 1: srce = RAM[stack.top()]; break;
		case 2: srce = RAM[srce]; break;
		case 3: srce = stack.top(); break;
		}
		switch (OPCODE)
		{
		case SET: if (((opByte & 12) >> 2) == 3) disk[stack.top()] = srce; else disk[static_cast<unsigned short>(RAM[dest] << 8) | RAM[dest + 1]] = srce; continue;
		case GET: DEST(disk[srce]) continue;
		case MOV: DEST(srce) continue;
		case CMP:
			flags[equal] = DEST_VAL == srce;
			flags[not_equal] = DEST_VAL != srce;
			flags[greater] = DEST_VAL > srce;
			flags[greater_equal] = DEST_VAL >= srce;
			flags[lower] = DEST_VAL < srce;
			flags[lower_equal] = DEST_VAL <= srce;
			continue;
		case ADD: DEST(DEST_VAL + srce); flags[zero] = DEST_VAL == 0; continue;
		case SUB: DEST(DEST_VAL - srce); flags[zero] = DEST_VAL == 0; continue;
		case MUL: DEST(DEST_VAL * srce); flags[zero] = DEST_VAL == 0; continue;
		case DIV: DEST(DEST_VAL / srce); flags[zero] = DEST_VAL == 0; continue;
		case MOD: DEST(DEST_VAL % srce); flags[zero] = DEST_VAL == 0; continue;
		case AND: DEST(DEST_VAL & (srce | 0xff00)); flags[zero] = DEST_VAL == 0; continue;
		}
	}
}
