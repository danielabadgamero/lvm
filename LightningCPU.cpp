#include "LightningCore.h"
#include "LightningCPU.h"

#define eval(n) (n > MAX_RAM ? REG[n - MAX_RAM] : n)

int Lightning::CPU::cycle(void*)
{
	while (!running);

	while (running)
	{
		switch (RAM[PC])
		{
		case HALT:
			running = false;
			break;
		case SET:
			eval(RAM[PC + 1]) = eval(RAM[PC + 2]);
			PC += 3;
			break;
		case RMEM:
			eval(RAM[PC + 1]) = RAM[eval(RAM[PC + 2])];
			PC += 3;
			break;
		case WMEM:
			RAM[eval(RAM[PC + 1])] = eval(RAM[PC + 2]);
			PC += 3;
			break;
		case ADD:
			eval(RAM[PC + 1]) += eval(RAM[PC + 2]);
			PC += 3;
			break;
		case SUB:
			eval(RAM[PC + 1]) -= eval(RAM[PC + 2]);
			PC += 3;
			break;
		case MUL:
			eval(RAM[PC + 1]) *= eval(RAM[PC + 2]);
			PC += 3;
			break;
		case DIV:
			eval(RAM[PC + 1]) /= eval(RAM[PC + 2]);
			PC += 3;
			break;
		case JMP:
			PC = eval(RAM[PC + 1]);
			break;
		case CMP:
			REG[FLAGS - MAX_RAM] = (eval(RAM[PC + 1]) == eval(RAM[PC + 2]));
			REG[FLAGS - MAX_RAM] = ((eval(RAM[PC + 1]) != eval(RAM[PC + 2])) << 1);
			REG[FLAGS - MAX_RAM] = ((eval(RAM[PC + 1]) > eval(RAM[PC + 2])) << 2);
			REG[FLAGS - MAX_RAM] = ((eval(RAM[PC + 1]) < eval(RAM[PC + 2])) << 3);
			REG[FLAGS - MAX_RAM] = ((eval(RAM[PC + 1]) >= eval(RAM[PC + 2])) << 4);
			REG[FLAGS - MAX_RAM] = ((eval(RAM[PC + 1]) <= eval(RAM[PC + 2])) << 5);
			PC += 3;
			break;
		case JEQ:
			if (REG[FLAGS - MAX_RAM] & FLAGS_EQ)
				PC = eval(RAM[PC + 1]);
			break;
		case JNE:
			if (REG[FLAGS - MAX_RAM] & FLAGS_NE)
				PC = eval(RAM[PC + 1]);
			break;
		case JGT:
			if (REG[FLAGS - MAX_RAM] & FLAGS_GT)
				PC = eval(RAM[PC + 1]);
			break;
		case JLT:
			if (REG[FLAGS - MAX_RAM] & FLAGS_LT)
				PC = eval(RAM[PC + 1]);
			break;
		case JGE:
			if (REG[FLAGS - MAX_RAM] & FLAGS_GE)
				PC = eval(RAM[PC + 1]);
			break;
		case JLE:
			if (REG[FLAGS - MAX_RAM] & FLAGS_LE)
				PC = eval(RAM[PC + 1]);
			break;
		case CALL:
			stack.push(PC + 1);
			PC = RAM[PC + 1];
			break;
		case RET:
			PC = stack.top();
			stack.pop();
			break;
		case PUSH:
			stack.push(eval(RAM[PC + 1]));
			break;
		case POP:
			REG[RAM[PC + 1]] = stack.top();
			stack.pop();
			break;
		}
	}

	return 0;
}