#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <bitset>

#include "LightningOP.h"
#include "LightningCore.h"

void Lightning::OP::loadOperations()
{
	operations.resize(total_opcodes);

	operations[SET] = []()
	{
		REG[PC->Rd.to_ulong()] = PC->imm;
	};

	operations[SIE] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] == REG[PC->Rs2.to_ulong()];
	};

	operations[SIG] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] > REG[PC->Rs2.to_ulong()];
	};

	operations[SIL] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] < REG[PC->Rs2.to_ulong()];
	};

	operations[ADD] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] + REG[PC->Rs2.to_ulong()];
	};

	operations[SUB] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] - REG[PC->Rs2.to_ulong()];
	};

	operations[MUL] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] * REG[PC->Rs2.to_ulong()];
	};

	operations[AND] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] & REG[PC->Rs2.to_ulong()];
	};

	operations[OR] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] | REG[PC->Rs2.to_ulong()];
	};

	operations[XOR] = []()
	{
		REG[PC->Rd.to_ulong()] = REG[PC->Rs1.to_ulong()] ^ REG[PC->Rs2.to_ulong()];
	};

	operations[NOT] = []()
	{
		REG[PC->Rd.to_ulong()] = ~REG[PC->Rs1.to_ulong()];
	};

	operations[WMEM] = []()
	{
		RAM[REG[PC->Rs2.to_ulong()]].imm = REG[PC->Rs1.to_ulong()];
	};

	operations[RMEM] = []()
	{
		REG[PC->Rd.to_ulong()] = RAM[REG[PC->Rs2.to_ulong()]].imm;
	};

	operations[JP] = []()
	{
		PC += PC->imm;
		PC--;
	};

	operations[JEQZ] = []()
	{
		if (REG[PC->Rs1.to_ulong()] == 0)
		{
			PC += PC->imm;
			PC--;
		}
	};

	operations[JNEZ] = []()
	{
		if (REG[PC->Rs1.to_ulong()] != 0)
		{
			PC += PC->imm;
			PC--;
		}
	};

	operations[CALL] = []()
	{
		stack.push((PC + 1) - RAM);
		PC = &RAM[REG[PC->Rs1.to_ulong()]] - 1;
	};

	operations[RET] = []()
	{
		PC = &RAM[stack.top()] - 1;
		stack.pop();
	};

	operations[HALT] = []()
	{
		PC = RAM;
		mode = Mode::CMD;
		while (PC->allocated)
		{
			PC->imm = 0;
			PC->opcode.reset();
			PC->Rd.reset();
			PC->Rs1.reset();
			PC->Rs2.reset();
			PC->allocated = false;
			PC++;
		}
		std::cout << "Press any key to continue...";
		while (!getchar());
		std::cout << "\n";
	};
}

void Lightning::OP::processOperation()
{
	operations[PC->opcode.to_ulong()]();
	PC++;
}