#include "Lightning.h"

void Lightning::Op::operator=(char byte)
{

}

void Lightning::reset()
{
	memset(RAM, 0, 1ll << 16);
	memset(reg, 0, 8);
	pc = pb = 0;
	flags.reset();
	while (!stack.empty()) stack.pop();
}

void Lightning::loop()
{
	op = RAM[pc];
	pc++;
}