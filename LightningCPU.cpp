#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ir.bitfields.opcode
#define rDest reg[ir.bitfields.reg]
#define rSource reg[ir.bitfields.op2]
#define imm8 static_cast<unsigned char>(ir.bitfields.op2)
#define imm10 static_cast<unsigned short>(ir.bitfields.reg << 8) + ir.bitfields.op2
#define aMode ir.bitfields.addrMode
#define value ((aMode == 0) ? rSource : imm8)
#define value10 ((aMode == 0) ? rSource : imm10)

static void readMemory(short address, short* dest)
{
	if (Lightning::Core::chipSelected)
		*dest = Lightning::Core::RAM[address];
	else
		*dest = Lightning::Core::ROM[address];
}

static void writeMemory(short address, short data)
{
	Lightning::Core::RAM[address] = static_cast<unsigned char>(data);
}

void Lightning::CPU::decode()
{
	switch (opcode)
	{
		//	Data management
	case UNUSED_1:
		break;
	case MOV:
		rDest = value;
		break;
	case SPB:
		pb = value;
		break;
	case PSHB:
		stack.push(pb);
		break;
	case POPB:
		pb = stack.top();
		stack.pop();
		break;
	case LD:
		readMemory(value + pb, &rDest);
		break;
	case ST:
		writeMemory(rDest, value + pb);
		break;
	case PUSH:
		stack.push(value);
		break;
	case POP:
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
			Core::chipSelected = imm8;
		pc = 0;
		break;
	case CMP:
		compFlags[equal] = rDest == value;
		compFlags[greater] = rDest > value;
		compFlags[greater_equal] = rDest >= value;
		compFlags[less] = rDest < value;
		compFlags[less_equal] = rDest <= value;
		break;
	case JMP:
		pc = value10 + pb;
		break;
	case JEQ:
		if (compFlags[equal])
			pc = value10 + pb;
		break;
	case JNE:
		if (compFlags[not_equal])
			pc = value10 + pb;
		break;
	case JGT:
		if (compFlags[greater])
			pc = value10 + pb;
		break;
	case JGE:
		if (compFlags[greater_equal])
			pc = value10 + pb;
		break;
	case JLT:
		if (compFlags[less])
			pc = value10 + pb;
		break;
	case JLE:
		if (compFlags[less_equal])
			pc = value10 + pb;
		break;
	case CALL:
		stack.push(pc);
		pc = value10 + pb;
		break;
	case RET:
		pc = stack.top();
		stack.pop();
		break;

		//	Arithmetic
	case ADD:
		rDest += value;
		break;
	case SUB:
		rDest -= value;
		break;
	case MUL:
		rDest *= value;
		break;
	case DIV:
		rDest /= value;
		break;
	case MOD:
		rDest %= value;
		break;
	case SHFT:
		rDest <<= value;
		break;

		//	Logic
	case AND:
		rDest &= value;
		break;
	case OR:
		rDest |= value;
		break;
	case XOR:
		rDest ^= value;
		break;
	case NOT:
		rDest = ~value;
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (!Core::running);

	while (Core::running)
	{
		short instruction{};
		for (int i{}; i != 2; i++)
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