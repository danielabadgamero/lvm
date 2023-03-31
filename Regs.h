#ifndef REGS_H
#define REGS_H

#include <map>
#include <string>

std::map<std::string, char> regs
{
	{ "ax", 0 },
	{ "bx", 1 },
	{ "cx", 2 },
	{ "dx", 3 },
	{ "pc", 4 },
	{ "pb", 5 },
	{ "tr", 6 },
};

#endif