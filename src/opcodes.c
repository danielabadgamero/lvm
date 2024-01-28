#include "opcodes.h"
#include "lvm.h"

#include <stdio.h>
#include <string.h>

static int get_source(int src, int reg, int size)
{
	int source;
	switch (src)
	{
	case 0:
		if (size == 0) source = RAM[pc++];
		else
		{
			source = (short)(RAM[pc] << 8) | RAM[pc + 1];
			pc += 2;
		}
		break;
	case 1:
		source = registers[reg];
		break;
	case 2:
		if (size == 0) source = RAM[registers[reg]];
		else source = (int)(RAM[registers[reg]] << 8) | RAM[registers[reg] + 1];
		break;
	case 3:
		short offset = (short)(RAM[pc] << 8) | RAM[pc + 1];
		pc += 2;
		if (size == 0) source = RAM[pc + offset];
		else source = (short)(RAM[pc + offset] << 8) | RAM[pc + offset + 1];
		break;
	}
	return source;
}

static int get_dest(int dst, int reg, int size)
{
	if (dst == 0) return registers[reg];
	else if (size == 0) return RAM[registers[reg]];
	else return (short)(RAM[registers[reg]] << 8) | RAM[registers[reg] + 1];
}

static void set_dest(int dst, int reg, int size, int val)
{
	if (dst == 0) registers[reg] = val;
	else if (size == 0) RAM[registers[reg]] = val;
	else
	{
		RAM[registers[reg]] = val >> 8;
		RAM[registers[reg] + 1] = val;
	}
}

void f0_MOV(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_source(src, r1, size));
}

void f0_MOVI(int, int, int, int, int)
{
}

void f0_ADD(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) + get_source(src, r1, size));
}

void f0_SUB(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) - get_source(src, r1, size));
}

void f0_MUL(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) * get_source(src, r1, size));
}

void f0_DIV(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) / get_source(src, r1, size));
}

void f0_MOD(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) % get_source(src, r1, size));
}

void f0_AND(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) & get_source(src, r1, size));
}

void f0_OR(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) | get_source(src, r1, size));
}

void f0_XOR(int size, int src, int dst, int r1, int r2)
{
	set_dest(dst, r2, size, get_dest(dst, r2, size) ^ get_source(src, r1, size));
}

void f0_EQ(int size, int src, int dst, int r1, int r2)
{
	T = get_dest(dst, r2, size) == get_source(src, r1, size);
}

void f0_GRT(int size, int src, int dst, int r1, int r2)
{
	T = get_dest(dst, r2, size) > get_source(src, r1, size);
}

void f0_LST(int size, int src, int dst, int r1, int r2)
{
	T = get_dest(dst, r2, size) < get_source(src, r1, size);
}

void f0_GEQ(int size, int src, int dst, int r1, int r2)
{
	T = get_dest(dst, r2, size) >= get_source(src, r1, size);
}

void f0_LEQ(int size, int src, int dst, int r1, int r2)
{
	T = get_dest(dst, r2, size) <= get_source(src, r1, size);
}

void f0_NEQ(int size, int src, int dst, int r1, int r2)
{
	T = get_dest(dst, r2, size) != get_source(src, r1, size);
}

void (*f0_opcodes[16])(int, int, int, int, int) = 
{
	f0_MOV, f0_MOVI, f0_ADD, f0_SUB, f0_MUL, f0_DIV, f0_MOD, f0_AND,
	f0_OR, f0_XOR, f0_EQ, f0_GRT, f0_LST, f0_GEQ, f0_LEQ, f0_NEQ
};



void f1_INC(int size, int dst, int reg)
{
	set_dest(dst, reg, size, get_dest(dst, reg, size) + 1);
}

void f1_POP(int size, int dst, int reg)
{
	set_dest(dst, reg, size, *data_stack_ptr);
	data_stack_ptr--;
}

void (*f1_opcodes[2])(int, int, int) =
{
	f1_INC, f1_POP
};



void f20_JMP(int imm)
{
	short offset = (short)imm | get_source(0, 0, 0);
	pc += offset;
}

void f20_JMT(int imm)
{
	short offset = (short)imm | get_source(0, 0, 0);
	if (T) pc += offset;
}

void f20_CALL(int imm)
{
	short offset = (short)imm | get_source(0, 0, 0);
	*(call_stack_ptr++) = pc;
	pc += offset;
}

void f20_DCALL(int imm)
{
	short addr = (short)imm | get_source(0, 0, 0);
	*(call_stack_ptr++) = pc;
	pc = addr;
}

void (*f20_opcodes[4])(int) =
{
	f20_JMP, f20_JMT, f20_CALL, f20_DCALL
};



void f21_HALT()
{
	running = 0;
}

void f21_ADD()
{
	*(data_stack_ptr - 1) += *data_stack_ptr;
	data_stack_ptr--;
}

void f21_SUB()
{
	*(data_stack_ptr - 1) -= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_MUL()
{
	*(data_stack_ptr - 1) *= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_DIV()
{
	*(data_stack_ptr - 1) /= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_MOD()
{
	*(data_stack_ptr - 1) %= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_AND()
{
	*(data_stack_ptr - 1) &= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_OR()
{
	*(data_stack_ptr - 1) |= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_XOR()
{
	*(data_stack_ptr - 1) ^= *data_stack_ptr;
	data_stack_ptr--;
}

void f21_EQ()
{
	T = *(data_stack_ptr - 1) == *data_stack_ptr;
}

void f21_GRT()
{
	T = *(data_stack_ptr - 1) > *data_stack_ptr;
}

void f21_LST()
{
	T = *(data_stack_ptr - 1) < *data_stack_ptr;
}

void f21_GEQ()
{
	T = *(data_stack_ptr - 1) >= *data_stack_ptr;
}

void f21_LEQ()
{
	T = *(data_stack_ptr - 1) <= *data_stack_ptr;
}

void f21_NEQ()
{
	T = *(data_stack_ptr - 1) != *data_stack_ptr;
}

void f21_LDB()
{
	*(data_stack_ptr - 1) = RAM[*data_stack_ptr];
	data_stack_ptr--;
}

void f21_LDW()
{
	*(data_stack_ptr - 1) = ((int)RAM[*data_stack_ptr] << 8) | RAM[*data_stack_ptr + 1];
	data_stack_ptr--;
}

void f21_INC()
{
	(*data_stack_ptr)++;
}

void f21_DEC()
{
	(*data_stack_ptr)--;
}

void f21_JMP()
{
	pc += *data_stack_ptr;
}

void f21_JMT()
{
	if (T) pc += *data_stack_ptr;
}

void f21_CALL()
{
	*(++call_stack_ptr) = pc;
	pc += *data_stack_ptr;
}

void f21_DCALL()
{
	*(++call_stack_ptr) = pc;
	pc = *data_stack_ptr;
}

void f21_NOT()
{
	*data_stack_ptr = ~(*data_stack_ptr);
}

void f21_IN()
{
	memcpy(RAM + ((registers[0] & 0xffff0000) >> 16), disk[registers[0] & 0xffff], 512);
}

void f21_OUT()
{
	memcpy(disk[registers[0] & 0xffff], RAM + ((registers[0] & 0xffff0000) >> 16), 512);
}

void f21_CIN()
{
	registers[0] = getchar();
}

void f21_COUT()
{
	putchar(registers[0]);
}

void (*f21_opcodes[32])() =
{
	f21_HALT, f21_ADD, f21_SUB, f21_MUL, f21_DIV, f21_MOD, f21_AND, f21_OR,
	f21_XOR, f21_EQ, f21_GRT, f21_LST, f21_GEQ, f21_LEQ, f21_NEQ, f21_LDB,
	f21_LDW, f21_INC, f21_DEC, f21_JMP, f21_JMT, f21_CALL, f21_DCALL, f21_NOT,
	f21_IN, f21_OUT, f21_CIN, f21_COUT
};



void f3_push(int size, int src, int reg)
{
	*(++data_stack_ptr) = get_source(src, reg, size);
}

void (*f3_opcodes[1])(int, int, int) =
{
	f3_push
};
