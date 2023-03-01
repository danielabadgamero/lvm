#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ir.bitfields.opcode
#define rDest reg[ir.bitfields.reg]
#define rSource reg[ir.bitfields.op2]
#define imm ir.bitfields.op2
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
		rDest = stack.top();
		stack.pop();
		break;
	case RDSK:
		break;
	case WDSK:
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
			pc = rSource;
		else
			pc = imm;
		break;
	case JEQ:
		if (compFlags[equal])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case JNE:
		if (compFlags[not_equal])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case JGT:
		if (compFlags[greater])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case JGE:
		if (compFlags[greater_equal])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case JLT:
		if (compFlags[less])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case JLE:
		if (compFlags[less_equal])
		{
			if (aMode == 0)
				pc = rSource;
			else
				pc = imm;
		}
		break;
	case CALL:
		stack.push(pc);
		if (aMode == 0)
			pc = rSource;
		else
			pc = imm;
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
		SDL_Log("Hello!");
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