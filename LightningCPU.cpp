#include <SDL.h>

#include "LightningCore.h"
#include "LightningCPU.h"

int Lightning::CPU::cycle(void*)
{
	while (!running);

	while (running)
	{
		REG[IR] = RAM[PC];
		REG[IR] <<= 8;
		REG[IR] += RAM[PC + 1];
		REG[IR] <<= 8;
		REG[IR] += RAM[PC + 2];
		REG[IR] <<= 8;
		REG[IR] += RAM[PC + 3];
		switch (REG[IR] >> 24)
		{
		case HALT:
			running = false;
			break;
		case SET:
			REG[(REG[IR] & Rd) >> 16] = REG[IR] & imm16;
			break;
		case SAC:
			REG[AC] = REG[IR] & imm24;
			break;
		case SAR:
			REG[AR] = REG[IR] & imm24;
			break;
		case SDR:
			REG[DR] = REG[IR] & imm24;
			break;
		case RMEM:
			REG[DR] = RAM[REG[AR]];
			break;
		case WMEM:
			RAM[REG[AR]] = static_cast<unsigned char>(REG[DR]);
			break;
		case INC:
			REG[(REG[IR] & Rd) >> 16] += static_cast<char>(REG[IR] & imm16);
			break;
		case JMP:
			REG[PC] = REG[(REG[IR] & Rs1) >> 8] * 4;
			break;
		case JPI:
			REG[PC] += static_cast<char>((REG[IR] & imm24) * 4 - 4);
			break;
		case JPZ:
			REG[PC] = ((REG[REG[IR] & Rs2] == 0) ? (REG[(REG[IR] & Rs1) >> 8] * 4) - 4 : Lightning::CPU.PC);
			break;
		case JPZI:
			Lightning::CPU.PC += ((REG[(REG[IR] & Rs1) >> 8] == 0) ? static_cast<char>((REG[IR] & imm8) * 4 - 4) : 0);
			break;
		case JNZ:
			Lightning::CPU.PC = ((REG[REG[IR] & Rs2] != 0) ? (REG[(REG[IR] & Rs1) >> 8] * 4) - 4 : Lightning::CPU.PC);
			break;
		case JNZI:
			Lightning::CPU.PC += ((REG[(REG[IR] & Rs1) >> 8] != 0) ? static_cast<char>((REG[IR] & imm8) * 4 - 4) : 0);
			break;
		case PUSH:
			stack.push(Lightning::CPU.PC / 4);
			break;
		case POP:
			REG[AR] = stack.top();
			stack.pop();
			break;
		case CALL:
			stack.push(Lightning::CPU.PC);
			Lightning::CPU.PC = REG[(REG[IR] & Rs1) >> 8] * 4 - 4;
			break;
		case RET:
			Lightning::CPU.PC = stack.top();
			stack.pop();
			break;
		default:
			REG[(REG[IR] & Rd) >> 16] = ALU.process(static_cast<int>(REG[IR] >> 24), REG[(REG[IR] & Rs1) >> 8], REG[(REG[IR] & Rs2)]);
		}
	}

	return 0;
}