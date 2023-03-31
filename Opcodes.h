#ifndef OPCODES_H
#define OPCODES_H

#include <map>
#include <string>

std::map<std::string, char> opcodes
{
	{ "HALT", 0 },
	{ "MOV", 1 },
	{ "MGT", 2 },
	{ "MLT", 3 },
	{ "MEQ", 4 },
	{ "POP", 5 },
	{ "RD", 6 },
	{ "WR", 7 },
	{ "IN", 8 },
	{ "OUT", 9 },
	{ "ADD", 10 },
	{ "MUL", 11 },
	{ "DIV", 12 },
	{ "CMP", 13 },
	{ "AND", 14 },
	{ "NOT", 15 },
};

#endif