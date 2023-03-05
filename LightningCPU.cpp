#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ir.bitfields.opcode
#define rDest reg[ir.bitfields.reg]
#define rSource reg[ir.bitfields.op2]
#define imm static_cast<int>(ir.bitfields.op2)
#define aMode ir.bitfields.addrMode

static void readMemory(int address, int* dest)
{
	if (Lightning::Core::chipSelected)
		*dest = Lightning::Core::RAM[address];
	else
		*dest = Lightning::Core::ROM[address];
}

static void writeMemory(int address, int data)
{
	Lightning::Core::RAM[address] = (unsigned char)data;
}

void Lightning::CPU::decode()
{
	switch (opcode)
	{
		//	Data management
	case MOV:
		if (aMode == 0)
			rDest = rSource;
		else
			rDest = imm;
		break;
	case LD:
		if (aMode == 0)
			readMemory(rSource, &rDest);
		else
			readMemory(imm, &rDest);
		break;
	case ST:
		if (aMode == 0)
			writeMemory(rDest, rSource);
		else
			writeMemory(rDest, imm);
		break;
	case PUSH:
		if (aMode == 0)
			stack.push(rSource);
		else
			stack.push(imm);
		break;
	case POP:
		if (aMode)
			rDest = stack.top();
		stack.pop();
		break;
	case IN:
		break;
	case OUT:
		break;
		
		//	Flow control
	case HALT:
		if (aMode == 0)
			Core::running = false;
		else
			Core::chipSelected = imm;
		break;
	case CMP:
		if (aMode == 0)
		{
			compFlags[equal] = rDest == rSource;
			compFlags[greater] = rDest > rSource;
			compFlags[greater_equal] = rDest >= rSource;
			compFlags[less] = rDest < rSource;
			compFlags[less_equal] = rDest <= rSource;
		}
		else
		{
			compFlags[equal] = rDest == imm;
			compFlags[greater] = rDest > imm;
			compFlags[greater_equal] = rDest >= imm;
			compFlags[less] = rDest < imm;
			compFlags[less_equal] = rDest <= imm;
		}
		break;
	case JMP:
		if (aMode == 0)
			switch (ir.bitfields.reg)
			{
			case 0:
				pc = rSource;
				break;
			case 1:
				pc += rSource - 4;
				break;
			case 2:
				pc = pb + rSource;
				break;
			case 3:
				readMemory(rSource, (int*)&pc);
				break;
			}
		else
			switch (ir.bitfields.reg)
			{
			case 0:
				pc = imm;
				break;
			case 1:
				pc += imm - 4;
				break;
			case 2:
				pc = pb + imm;
				break;
			case 3:
				readMemory(imm, (int*)&pc);
				break;
			}
		break;
	case JEQ:
		if (compFlags[equal])
		{
			if (aMode == 0)
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = rSource;
					break;
				case 1:
					pc += rSource - 4;
					break;
				case 2:
					pc = pb + rSource;
					break;
				case 3:
					readMemory(rSource, (int*)&pc);
					break;
				}
			else
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = imm;
					break;
				case 1:
					pc += imm - 4;
					break;
				case 2:
					pc = pb + imm;
					break;
				case 3:
					readMemory(imm, (int*)&pc);
					break;
				}
			break;
		}
		break;
	case JNE:
		if (compFlags[not_equal])
		{
			if (aMode == 0)
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = rSource;
					break;
				case 1:
					pc += rSource - 4;
					break;
				case 2:
					pc = pb + rSource;
					break;
				case 3:
					readMemory(rSource, (int*)&pc);
					break;
				}
			else
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = imm;
					break;
				case 1:
					pc += imm - 4;
					break;
				case 2:
					pc = pb + imm;
					break;
				case 3:
					readMemory(imm, (int*)&pc);
					break;
				}
			break;
		}
		break;
	case JGT:
		if (compFlags[greater])
		{
			if (aMode == 0)
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = rSource;
					break;
				case 1:
					pc += rSource - 4;
					break;
				case 2:
					pc = pb + rSource;
					break;
				case 3:
					readMemory(rSource, (int*)&pc);
					break;
				}
			else
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = imm;
					break;
				case 1:
					pc += imm - 4;
					break;
				case 2:
					pc = pb + imm;
					break;
				case 3:
					readMemory(imm, (int*)&pc);
					break;
				}
			break;
		}
		break;
	case JGE:
		if (compFlags[greater_equal])
		{
			if (aMode == 0)
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = rSource;
					break;
				case 1:
					pc += rSource - 4;
					break;
				case 2:
					pc = pb + rSource;
					break;
				case 3:
					readMemory(rSource, (int*)&pc);
					break;
				}
			else
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = imm;
					break;
				case 1:
					pc += imm - 4;
					break;
				case 2:
					pc = pb + imm;
					break;
				case 3:
					readMemory(imm, (int*)&pc);
					break;
				}
			break;
		}
		break;
	case JLT:
		if (compFlags[less])
		{
			if (aMode == 0)
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = rSource;
					break;
				case 1:
					pc += rSource - 4;
					break;
				case 2:
					pc = pb + rSource;
					break;
				case 3:
					readMemory(rSource, (int*)&pc);
					break;
				}
			else
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = imm;
					break;
				case 1:
					pc += imm - 4;
					break;
				case 2:
					pc = pb + imm;
					break;
				case 3:
					readMemory(imm, (int*)&pc);
					break;
				}
			break;
		}
		break;
	case JLE:
		if (compFlags[less_equal])
		{
			if (aMode == 0)
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = rSource;
					break;
				case 1:
					pc += rSource - 4;
					break;
				case 2:
					pc = pb + rSource;
					break;
				case 3:
					readMemory(rSource, (int*)&pc);
					break;
				}
			else
				switch (ir.bitfields.reg)
				{
				case 0:
					pc = imm;
					break;
				case 1:
					pc += imm - 4;
					break;
				case 2:
					pc = pb + imm;
					break;
				case 3:
					readMemory(imm, (int*)&pc);
					break;
				}
			break;
		}
		break;
	case CALL:
		stack.push(pc);
		if (aMode == 0)
			switch (ir.bitfields.reg)
			{
			case 0:
				pc = rSource;
				break;
			case 1:
				pc += rSource - 4;
				break;
			case 2:
				pc = pb + rSource;
				break;
			case 3:
				readMemory(rSource, (int*)&pc);
				break;
			}
		else
			switch (ir.bitfields.reg)
			{
			case 0:
				pc = imm;
				break;
			case 1:
				pc += imm - 4;
				break;
			case 2:
				pc = pb + imm;
				break;
			case 3:
				readMemory(imm, (int*)&pc);
				break;
			}
		break;
		break;
	case RET:
		pc = stack.top();
		stack.pop();
		break;
	case INT:
		interruptTable[imm]();
		break;

		//	Arithmetic
	case ADD:
		if (aMode == 0)
			rDest += rSource;
		else
			rDest += imm;
		break;
	case SUB:
		if (aMode == 0)
			rDest -= rSource;
		else
			rDest -= imm;
		break;
	case MUL:
		if (aMode == 0)
			rDest *= rSource;
		else
			rDest *= imm;
		break;
	case DIV:
		if (aMode == 0)
			rDest /= rSource;
		else
			rDest /= imm;
		break;
	case MOD:
		if (aMode == 0)
			rDest %= rSource;
		else
			rDest %= imm;
		break;
	case SHFT:
		if (aMode == 0)
			rDest >>= rSource;
		else
			rDest >>= imm;
		break;

		//	Logic
	case AND:
		if (aMode == 0)
			rDest &= rSource;
		else
			rDest &= imm;
		break;
	case NAND:
		if (aMode == 0)
			rDest = ~(rSource & rDest);
		else
			rDest = ~(imm & rDest);
		break;
	case OR:
		if (aMode == 0)
			rDest |= rSource;
		else
			rDest |= imm;
		break;
	case XOR:
		if (aMode == 0)
			rDest ^= rSource;
		else
			rDest ^= imm;
		break;
	case NOR:
		if (aMode == 0)
			rDest = ~(rSource | rDest);
		else
			rDest = ~(imm | rDest);
		break;
	case XNOR:
		if (aMode == 0)
			rDest = ~(rSource ^ rDest);
		else
			rDest = ~(imm ^ rDest);
		break;
	case NOT:
		if (aMode == 0)
			rDest = ~rSource;
		else
			rDest = ~imm;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	interruptTable[print_char] = []()
	{
		char c{ static_cast<char>(reg[0]) };

		for (int i{}; i != 32; i++)
		{
			for (int j{}; j != 3; j++)
			{
				for (int k{}; k != 8; k++)
				{
					Core::RAM[VIDEO + (cursor.y + i) * pixelsPitch + cursor.x * 3 + j * 24 + k * 3] ^= ((Core::font[c * 96 + i * 3 + j] & (1 << (7 - k))) >> (7 - k)) * 0xff;
					Core::RAM[VIDEO + (cursor.y + i) * pixelsPitch + cursor.x * 3 + j * 24 + k * 3 + 1] ^= ((Core::font[c * 96 + i * 3 + j] & (1 << (7 - k))) >> (7 - k)) * 0xff;
					Core::RAM[VIDEO + (cursor.y + i) * pixelsPitch + cursor.x * 3 + j * 24 + k * 3 + 2] ^= ((Core::font[c * 96 + i * 3 + j] & (1 << (7 - k))) >> (7 - k)) * 0xff;
				}
			}
		}

		if (cursor.x >= 1896)
		{
			cursor.y += 32;
			cursor.x = 0;
		}
		else
			cursor.x += 24;
	};

	while (!Core::running);

	while (Core::running)
	{
		int instruction{};
		for (int i{}; i != 4; i++)
		{
			readMemory(pc, &instruction);
			ir.instruction <<= 8;
			ir.instruction += instruction;
			pc++;
		}
		decode();
	}

	return 0;
}