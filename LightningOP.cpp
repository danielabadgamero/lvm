#include <vector>
#include <iostream>
#include <conio.h>

#include "LightningOP.h"
#include "LightningCore.h"

void Lightning::OP::loadOperations()
{
	operations.resize(total_opcodes);

	operations[HALT] = []()
	{
		running = false;
	};

	operations[SET] = []()
	{
		*instruction.Rd = instruction.imm;
	};

	operations[SEQ] = []()
	{
		*instruction.Rd = *instruction.Rs1 == *instruction.Rs2;
	};

	operations[SEQI] = []()
	{
		*instruction.Rd = *instruction.Rs1 == instruction.imm;
	};

	operations[SGTI] = []()
	{
		*instruction.Rd = *instruction.Rs1 > instruction.imm;
	};

	operations[ADD] = []()
	{
		*instruction.Rd = *instruction.Rs1 + *instruction.Rs2;
	};

	operations[ADDI] = []()
	{
		*instruction.Rd = *instruction.Rs1 + instruction.imm;
	};

	operations[MUL] = []()
	{
		*instruction.Rd = *instruction.Rs1 * *instruction.Rs2;
	};

	operations[MULI] = []()
	{
		*instruction.Rd = *instruction.Rs1 * instruction.imm;
	};

	operations[CALL] = []()
	{
		stack.push(*PC);
		*PC = *instruction.Rs1 - 1;
	};

	operations[RET] = []()
	{
		*PC = stack.top();
		stack.pop();
	};

	operations[JP] = []()
	{
		*PC += instruction.imm - 1;
	};

	operations[JT] = []()
	{
		if (*instruction.Rs1 != 0)
			*PC += instruction.imm - 1;
	};

	operations[JF] = []()
	{
		if (*instruction.Rs1 == 0)
			*PC += instruction.imm - 1;
	};

	operations[RMEM] = []()
	{
		*instruction.Rd = RAM[*instruction.Rs1];
	};

	operations[WMEM] = []()
	{
		RAM[*instruction.Rs2] = *instruction.Rs1;
	};

	operations[COUT] = []()
	{
		std::cout << static_cast<char>(*instruction.Rs1);
	};

	operations[COUTI] = []()
	{
		std::cout << static_cast<char>(instruction.imm);
	};

	operations[IOUT] = []()
	{
		std::cout << static_cast<int>(*instruction.Rs1);
	};

	operations[IN] = []()
	{
		*instruction.Rd = _getch();
	};
}

void Lightning::OP::processOperation()
{
	instruction.opcode = (RAM[*PC] & opcode) >> 27;
	instruction.Rd = REG + ((RAM[*PC] & Rd) >> 24);
	instruction.Rs1 = REG + ((RAM[*PC] & Rs1) >> 20);
	instruction.Rs2 = REG + ((RAM[*PC] & Rs2) >> 16);
	instruction.imm = (RAM[*PC] & imm);
	operations[instruction.opcode]();
	*PC += 1;
}