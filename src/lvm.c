#include "lvm.h"
#include "opcodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORMAT(b)	((b & 0xC0) >> 6)
#define FORMAT_2(b)	((b & 0x20) >> 5)

#define F0_OPCODE(w)	((w & 0x3C00) >> 10)
#define F0_SIZE(w)	((w & 0x0200) >> 9)
#define F0_SRC(w)	((w & 0x0180) >> 7)
#define F0_DST(w)	((w & 0x0040) >> 6)
#define F0_SREG(w)	((w & 0x0038) >> 3)
#define F0_DREG(w)	(w & 0x0007)

#define F1_OPCODE(b)	((b & 0x20) >> 5)
#define F1_SIZE(b)	((b & 0x10) >> 4)
#define F1_DST(b)	((b & 0x08) >> 3)
#define F1_REG(b)	(b & 0x07)

#define F20_OPCODE(b)	((b & 0x18) >> 3)
#define F20_IMM(b)	((short)(b & 0x07) << 8)

#define F21_OPCODE(b)	(b & 0x1f)

#define F3_SIZE(b)	((b & 0x20) >> 5)
#define F3_SRC(b)	((b & 0x18) >> 3)
#define F3_REG(b)	(b & 0x07)

int running = 0;
unsigned short pc = 0;
int registers[8] = {};
int T = 0;
unsigned char* RAM = NULL;
unsigned char disk[DISK_SIZE][512] = {{}};
unsigned short call_stack[1000] = {};
unsigned short* call_stack_ptr = call_stack;
int data_stack[1000] = {};
int* data_stack_ptr = data_stack;

static size_t fsize(FILE* file)
{
	size_t pos = ftell(file);
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, pos, SEEK_SET);
	return size;
}

void lvm_init(const char* disk_path)
{
	RAM = malloc(RAM_SIZE);

	FILE* bin = fopen(disk_path, "rb");

	size_t size = fsize(bin);
	if (size != (1 << 16) * 512)
	{
		printf("Disk file is not 2^16 * 512 bytes.");
		fclose(bin);
		return;
	}

	fread(disk, 512, 1 << 16, bin);

	memcpy(RAM, disk[0], 512);

	fclose(bin);

	running = 1;
}

void lvm_exec()
{
	unsigned int fmt = FORMAT(RAM[pc]);
	unsigned short ins;

	switch (fmt)
	{
	case 0:
		ins = ((unsigned short)RAM[pc] << 8) | RAM[pc + 1];
		pc += 2;
		f0_opcodes[F0_OPCODE(ins)](
			F0_SIZE(ins),
			F0_SRC(ins),
			F0_DST(ins),
			F0_SREG(ins),
			F0_DREG(ins));
		break;
	case 1:
		ins = RAM[pc++];
		f1_opcodes[F1_OPCODE(ins)](
			F1_SIZE(ins),
			F1_DST(ins),
			F1_REG(ins));
		break;
	case 2:
		ins = RAM[pc++];
		switch (FORMAT_2(ins))
		{
		case 0:
			f20_opcodes[F20_OPCODE(ins)](F20_IMM(ins));
			break;
		case 1:
			f21_opcodes[F21_OPCODE(ins)]();
			break;
		}
		break;
	case 3:
		ins = RAM[pc++];
		f3_opcodes[0](
			F3_SIZE(ins),
			F3_SRC(ins),
			F3_REG(ins));
		break;
	}
	//printf("Instruction: %d PC: %d R0: %d\n", ins, pc, registers[0]);
}

void lvm_quit()
{
	free(RAM);
}
