#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ((reg[ir] & 0xff000000) >> 24)
#define rd ((reg[ir] & 0x00ff0000) >> 16)
#define rs1 ((reg[ir] & 0x0000ff00) >> 8)
#define rs2 (reg[ir] & 0x000000ff)
#define imm16 (reg[ir] & 0x0000ffff)


void Lightning::CPU::decode()
{
	switch (opcode)
	{
	case HALT:
		bistables[running].flip();
		break;
	case MOVR:
		reg[rd] = reg[rs1];
		break;
	case MOVI:
		reg[rd] = imm16;
		break;
	case MVRC:
		if (bistables[rs2])
			reg[rd] = reg[rs1];
		break;
	case LWR:
		reg[rd] = RAM[reg[rs1]];
		break;
	case LWI:
		reg[rd] = RAM[imm16];
		break;
	case SWR:
		RAM[reg[rd]] = static_cast<char>(reg[rs1]);
		break;
	case SWI:
		RAM[reg[rd]] = static_cast<char>(imm16);
		break;
	case ADD:
		reg[rd] += reg[rs1];
		break;
	case ADDI:
		reg[rd] += imm16;
		break;
	case IN:
		reg[rd] = peripherals[rs1][rs2];
		break;
	case OUT:
		peripherals[rd][rs1] = reg[rs2];
		break;
	case CMP:
		bistables[CPU::equal] = reg[rs1] == reg[rs2];
		bistables[CPU::not_equal] = reg[rs1] != reg[rs2];
		bistables[CPU::greater] = reg[rs1] > reg[rs2];
		bistables[CPU::greater_equal] = reg[rs1] >= reg[rs2];
		bistables[CPU::less] = reg[rs1] < reg[rs2];
		bistables[CPU::less_equal] = reg[rs1] <= reg[rs2];
		break;
	case CMPI:
		bistables[CPU::equal] = reg[rd] == imm16;
		bistables[CPU::not_equal] = reg[rd] != imm16;
		bistables[CPU::greater] = reg[rd] > imm16;
		bistables[CPU::greater_equal] = reg[rd] >= imm16;
		bistables[CPU::less] = reg[rd] < imm16;
		bistables[CPU::less_equal] = reg[rd] <= imm16;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (~bistables[running])
	{
		reg[ir] = (static_cast<Uint8>(ROM[reg[pc]]) << 24) + (static_cast<Uint8>(ROM[reg[pc] + 1]) << 16) + (static_cast<Uint8>(ROM[reg[pc] + 2]) << 8) + static_cast<Uint8>(ROM[reg[pc] + 3]);
		reg[pc] += 4;
		decode();
	}
	
	reg[pc] = 0;

	while (bistables[running])
	{
		reg[ir] = (static_cast<Uint8>(RAM[reg[pc]]) << 24) + (static_cast<Uint8>(RAM[reg[pc] + 1]) << 16) + (static_cast<Uint8>(RAM[reg[pc] + 2]) << 8) + static_cast<Uint8>(RAM[reg[pc] + 3]);
		reg[pc] += 4;
		decode();
	}

	return 0;
}