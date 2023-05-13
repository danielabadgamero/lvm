#include "Lightning.h"

void Lightning::Op::operator=(char byte)
{
	opcode = (byte & 0xf0) >> 4;
	dest = (byte & 0x08) >> 3;
	ddMode = (byte & 0x04) >> 2;
	src = (byte & 0x02) >> 1;
	sdMode = byte & 0x01;
}

void Lightning::reset()
{
	memset(RAM, 0, 1ll << 16);
	memset(reg, 0, 8);
	pc = pb = 0;
	flag.reset();
	while (!stack.empty()) stack.pop();
}

void Lightning::loop()
{
	op = RAM[pc];
	pc++;
	if (op.dest)
	{
	}
}