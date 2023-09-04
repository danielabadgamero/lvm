#include "VM.h"
#include "Core.h"
#include "Dev.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#define OPCODE ((opByte & 0xf0) >> 4)

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
	std::filesystem::path sym_path{ prog_path / "sym" };
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

	std::filesystem::path bin_path{ prog_path / "bin" };
	std::ifstream bin{ bin_path.string(), std::ios::binary };
	size_t size{ std::filesystem::file_size(bin_path) };
	if (size > (1 << 16))
	{
		std::cout << "Programme is too big" << std::endl;
		return;
	}

	bin.read((char*)RAM, size);

	Core::executing = true;
}

static unsigned short getSource(unsigned char opByte, unsigned short srce)
{
	unsigned short res{};

	switch (opByte & 0b110)
	{
	case 0b000: res = srce; break;
	case 0b010:
		if (opByte & 1) res = (VM::RAM[srce] << 8) | VM::RAM[srce + 1];
		else res = VM::RAM[srce];
		break;
	case 0b100:
		res = (VM::RAM[srce] << 8) | VM::RAM[srce + 1];
		if (opByte & 1) res = (VM::RAM[res] << 8) | VM::RAM[res + 1];
		else res = VM::RAM[res];
		break;
	}

	return res;
}

static void writeDest(unsigned short opByte, unsigned short dest, unsigned short srce)
{
	unsigned short addr{ dest };
	if (opByte & 0b1000) addr = (VM::RAM[addr] << 8) | VM::RAM[addr + 1];
	if (opByte & 1)
	{
		VM::RAM[addr] = (srce & 0xff00) >> 8;
		VM::RAM[addr + 1] = srce & 0xff;
	}
	else VM::RAM[addr] = srce;
}

static unsigned short getDest(unsigned short opByte, unsigned short dest)
{
	unsigned short val{ dest };
	if (opByte & 0b1000) val = (VM::RAM[val] << 8) | VM::RAM[val + 1];
	if (opByte & 1) return (VM::RAM[val] << 8) | VM::RAM[val + 1];
	else return VM::RAM[val];
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

		// std::cout << std::hex << pc - 1 << ' ' << std::find_if(opcodes.begin(), opcodes.end(), [&](const std::pair<std::string, unsigned char>& p){ return p.second == OPCODE; })->first << std::endl;

		switch (OPCODE)
		{
		case HLT:
			if ((opByte & 1) == 0)
			{
				Core::executing = false;
				for (unsigned char& b : RAM) b = 0;
				prog_path = "";
				prog_name.clear();
				return;
			}
			else running = false;
			continue;
		case RET:
			pc = call_stack.top();
			call_stack.pop();
			continue;
		case SET:
			dev = (opByte & 0b1100) >> 2;
			fun = opByte & 0b11;
			continue;
		}

		// bit	desc
		// 0	srce and dest: 1 byte (0) / 2 bytes (1)
		// 1-2	srce: imm(0) / RAM(1) / RAM[RAM](2)
		// 3	dest: RAM(0) / RAM[RAM](1)
		// 4-7	opcode

		unsigned short dest{ nextShort() };
		switch (OPCODE)
		{
		case CAL: call_stack.push(pc); pc = dest; continue;
		case PSH: stack.push(getSource(opByte, dest)); continue;
		case POP: writeDest(opByte, dest, stack.top()); stack.pop(); continue;
		case JMP: if (flags[opByte & 0xf]) pc = dest; continue;
		case NOT: writeDest(opByte, dest, ~getDest(opByte, dest)); continue;
		}

		unsigned short srce{ nextShort() };
		srce = getSource(opByte, srce);
		unsigned short destVal{ getDest(opByte, dest) };
		switch (OPCODE)
		{
		case SIG: Dev::devices[dev][fun](destVal, srce); continue;
		case MOV: writeDest(opByte, dest, srce); continue;
		case CMP:
			flags[equal] = destVal == srce;
			flags[not_equal] = destVal != srce;
			flags[greater] = destVal > srce;
			flags[greater_equal] = destVal >= srce;
			flags[lower] = destVal < srce;
			flags[lower_equal] = destVal <= srce;
			continue;
		case ADD: writeDest(opByte, dest, destVal + srce); flags[zero] = destVal == 0; continue;
		case SUB: writeDest(opByte, dest, destVal - srce); flags[zero] = destVal == 0; continue;
		case MUL: writeDest(opByte, dest, destVal * srce); flags[zero] = destVal == 0; continue;
		case DIV: writeDest(opByte, dest, destVal / srce); flags[zero] = destVal == 0; continue;
		case AND: writeDest(opByte, dest, destVal & srce); flags[zero] = destVal == 0; continue;
		}
	}
}
