#ifndef OPCODES_H
#define OPCODES_H

#include <vector>
#include <string>

std::vector<std::string> opcodes
{
	"SWTCH",
	"MOV",
	"MGT",
	"MLT",
	"MEQ",
	"POP",
	"RD",
	"WR",
	"IN",
	"OUT",
	"ADD",
	"MUL",
	"DIV",
	"CMP",
	"AND",
	"NOT"
};

enum Opcode
{
	SWTCH,	//	set selected chip to dAddr

	// Data management
	MOV,	//	move source to dAddr
	MGT,	//	MOV if greater flag set
	MLT,	//	MOV if less flag set
	MEQ,	//	MOV if equal flag set
	POP,	//	pop top of stack to dAddr
	RD,		//	read at address in source to dAddr
	WR,		//	write source to address in dAddr
	IN,		//	TBD
	OUT,	//	TBD

	// Arithmetic and logic
	ADD,	//	add source to dAddr
	MUL,	//	multiply dAddr times source
	DIV,	//	divide dAddr by source
	CMP,	//	compare dAddr with source and set flags
	AND,	//	set dAddr to its bitwise and with source
	NOT,	//	set dAddr to its bitwise negation
};

#endif