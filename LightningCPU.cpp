#include "LightningCore.h"
#include "LightningCPU.h"

#define opcode ir.bitfields.opcode
#define address ir.bitfields.address
#define operand ir.bitfields.operand

static void readMemory(int addr, int* dest)
{
	if (Lightning::Core::chipSelected)
		*dest = Lightning::Core::RAM[addr];
	else
		*dest = Lightning::Core::ROM[addr];
}

static void writeMemory(int addr, int data)
{
	Lightning::Core::RAM[addr] = static_cast<unsigned char>(data);
}

void Lightning::CPU::decode()
{
	switch (opcode)
	{
	}
}

int Lightning::CPU::cycle(void*)
{
	while (!Core::running);

	while (Core::running)
	{
		int instruction{};
		for (int i{}; i != 4; i++)
		{
			readMemory(regs[pc], &instruction);
			ir.instruction <<= 8;
			ir.instruction += instruction;
			regs[pc]++;
		}
		decode();
	}

	return 0;
}