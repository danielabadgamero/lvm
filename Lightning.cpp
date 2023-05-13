#include "Lightning.h"

void Lightning::reset()
{
	memset(RAM, 0, 1ll << 16);
	memset(reg, 0, 4);
	pc = 0;
	flags.reset();
	while (!stack.empty()) stack.pop();
}

void Lightning::loop()
{

}