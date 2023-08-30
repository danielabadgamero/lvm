#include "VM.h"
#include "Core.h"
#include "Dev.h"

#include <fstream>
#include <iostream>

#define OPCODE ((opByte & 0xf0) >> 4)
#define DEST RAM[dest]

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

	Core::executing = true;
}

void VM::execute(const std::string& command)
{
	pc = commands[command];
	running = true;

	while (running)
	{
		unsigned char opByte{ RAM[pc++] };

		switch (OPCODE)
		{
		case HLT: Core::executing = false;
		{	for (unsigned char& b : RAM) b = 0;
			std::ofstream disk_out{ (prog_path / "disk").string(), std::ios::binary };
			disk_out.write((char*)disk, sizeof(disk));
			prog_path = "";
		}	prog_name.clear();
			return;
		case RET: running = false; return;
		}

		// bit	desc
		// 0-1	src2: imm(0) / RAM(1) / disk(2) / RAM[RAM](3)
		// 2-3	src1: imm(0) / RAM(1) / disk(2) / RAM[RAM](3)
		// 4-7	opcode

		unsigned short dest{ nextShort() };

		switch (OPCODE)
		{
		case JMP: pc = dest; continue;
		case NOT: DEST = ~DEST; continue;
		case SND: Dev::devices[(dest & 0xff00) >> 8](dest & 0xff); continue;
		}

		unsigned short src1{ nextShort() };
		switch (opByte & 12)
		{
		case 1: src1 = RAM[src1]; break;
		case 2: src1 = disk[src1]; break;
		case 3: src1 = RAM[RAM[src1]]; break;
		}

		switch (OPCODE)
		{
		case STO: disk[dest] = src1; continue;
		case MOV: DEST = src1; continue;
		case ADD: DEST += src1; continue;
		case SUB: DEST -= src1; continue;
		case MUL: DEST *= src1; continue;
		case DIV: DEST /= src1; continue;
		case MOD: DEST %= src1; continue;
		case AND: DEST &= src1; continue;
		case NOR: DEST = ~(src1 | DEST); continue;
		}

		unsigned short src2{ nextShort() };
		switch (opByte & 3)
		{
		case 1: src2 = RAM[src2]; break;
		case 2: src2 = disk[src2]; break;
		case 3: src2 = RAM[RAM[src2]]; break;
		}

		switch (OPCODE)
		{
		case JGT: if (src1 > src2) pc = dest; continue;
		case JEQ: if (src1 == src2) pc = dest; continue;
		}
	}
}
