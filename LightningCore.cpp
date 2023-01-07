#include <iostream>
#include <fstream>
#include <conio.h>

#include "LightningCore.h"
#include "LightningFS.h"

#undef OUT

void Lightning::init()
{
	do
	{
		CPU.REG[IR] = ROM[CPU.PC];
		CPU.REG[IR] <<= 8;
		CPU.REG[IR] += ROM[CPU.PC + 1];
		CPU.REG[IR] <<= 8;
		CPU.REG[IR] += ROM[CPU.PC + 2];
		CPU.REG[IR] <<= 8;
		CPU.REG[IR] += ROM[CPU.PC + 3];
 		CPU.process();
		CPU.PC += 4;
	} while (CPU.REG[IR]);

	CPU.PC = 0;
	running = true;
}

void Lightning::CPU::process()
{
	switch (REG[IR] >> 24)
	{
	case HALT:
		running = false;
		break;
	case SET:
		REG[(REG[IR] & Rd) >> 16] = REG[IR] & imm16;
		break;
	case SAC:
		REG[AC] = REG[IR] & imm24;
		break;
	case SAR:
		REG[AR] = REG[IR] & imm24;
		break;
	case SDR:
		REG[DR] = REG[IR] & imm24;
		break;
	case COUTI:
		std::cout << static_cast<char>((REG[IR] & imm24) >> 16) << static_cast<char>((REG[IR] & imm24) >> 8) << static_cast<char>(REG[IR] & imm24);
		break;
	case COUT:
		std::cout << static_cast<char>(REG[(REG[IR] & Rs1) >> 8]);
		break;
	case IOUTI:
		std::cout << (REG[IR] & imm24);
		break;
	case IOUT:
		std::cout << (REG[(REG[IR] & Rs1) >> 8]);
		break;
	case RMEM:
		REG[DR] = RAM[REG[AR]];
		break;
	case WMEM:
		RAM[REG[AR]] = static_cast<unsigned char>(REG[DR]);
		break;
	case INC:
		REG[(REG[IR] & Rd) >> 16] += static_cast<char>(REG[IR] & imm16);
		break;
	case CIN:
		REG[(REG[IR] & Rd) >> 16] = _getch();
		break;
	case RFS:
		REG[DR] = FS::filesystem[REG[AR]];
		break;
	case WFS:
		FS::filesystem[REG[AR]] = static_cast<char>(REG[DR]);
		break;
	case JMP:
		Lightning::CPU.PC = REG[(REG[IR] & Rs1) >> 8] * 4;
		break;
	case JPI:
		Lightning::CPU.PC += static_cast<char>((REG[IR] & imm24) * 4 - 4);
		break;
	case JPZ:
		Lightning::CPU.PC = ((REG[REG[IR] & Rs2] == 0) ? (REG[(REG[IR] & Rs1) >> 8] * 4) - 4 : Lightning::CPU.PC);
		break;
	case JPZI:
		Lightning::CPU.PC += ((REG[(REG[IR] & Rs1) >> 8] == 0) ? static_cast<char>((REG[IR] & imm8) * 4 - 4) : 0);
		break;
	case JNZ:
		Lightning::CPU.PC = ((REG[REG[IR] & Rs2] != 0) ? (REG[(REG[IR] & Rs1) >> 8] * 4) - 4 : Lightning::CPU.PC);
		break;
	case JNZI:
		Lightning::CPU.PC += ((REG[(REG[IR] & Rs1) >> 8] != 0) ? static_cast<char>((REG[IR] & imm8) * 4 - 4) : 0);
		break;
	case PUSH:
		stack.push(Lightning::CPU.PC / 4);
		break;
	case POP:
		REG[AR] = stack.top();
		stack.pop();
		break;
	case CALL:
		stack.push(Lightning::CPU.PC);
		Lightning::CPU.PC = REG[(REG[IR] & Rs1) >> 8] * 4 - 4;
		break;
	case RET:
		Lightning::CPU.PC = stack.top();
		stack.pop();
		break;
	default:
		REG[(REG[IR] & Rd) >> 16] = ALU.process(static_cast<int>(REG[IR] >> 24), REG[(REG[IR] & Rs1) >> 8], REG[(REG[IR] & Rs2)]);
	}
}

long long Lightning::CPU::ALU::process(int opcode, long long rs1, long long rs2)
{
	switch (opcode)
	{
	case ADD:	return rs1 + rs2;
	case SUB:	return rs1 - rs2;
	case MUL:	return rs1 * rs2;
	case DIV:	return rs1 / rs2;
	case MOD:	return rs1 % rs2;
	case AND:	return rs1 & rs2;
	case OR:	return rs1 | rs2;
	case XOR:	return rs1 ^ rs2;
	case NOT:	return ~rs1;
	case CPY:	return rs1;
	case SEQ:	return rs1 == rs2;
	case SGT:	return rs1 > rs2;
	case SLT:	return rs1 < rs2;
	case LSFT:	return rs1 << rs2;
	case RSFT:	return rs1 >> rs2;

	default:	return -1;
	}
}