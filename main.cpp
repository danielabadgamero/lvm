#include <iostream>
#include <fstream>
#include <vector>

typedef void(*opcodeFunc)(std::ofstream*, std::string);

enum Opcode
{
	//			Description (addrMode = 0)			Description (addrMode = 1)

	//	Data management
	MOV,	//	Load reg op2 to rDest				Load imm to rDest
	LD,		//	Load reg addr op2 to rDest			Load addr op2 to rDest
	ST,		//	Store reg op2 in addr rDest			Store imm op2 in addr rDest
	PUSH,	//	Push reg op2 to stack				Push imm op2 to stack
	POP,	//	Pop top of stack					Pop top of stack into rDest
	IN,		//	TBD: communication with peripherals.
	OUT,	//	TBD: communication with peripherals.

	//	Flow control
	//	value in rDest changes behaviour of pc change:
	//	rDest	Desc
	//	00		Absolute address
	//	01		Relative to pc (programme counter)
	//	10		Relative to pb (programme base)
	//	11		Absolute indirect (2 jumps)
	HALT,	//	Finish execution					Switch to chip in op2
	CMP,	//	Eval rDest and reg op2 to flags		Eval rDest and imm op2 to flags
	JMP,	//	Set pc to reg op2					Set pc to imm op2
	JEQ,	//	Set pc to reg op2 if equal flag		Set pc to imm op2 if equal flag
	JNE,	//	Set pc to reg op2 if not equal flag	Set pc to imm op2 if not equal flag
	JGT,	//	Set pc to reg op2 if greater flag	Set pc to imm op2 if greater flag
	JGE,	//	Set pc to reg op2 if grt eq flag	Set pc to imm op2 if grt eq flag
	JLT,	//	Set pc to reg op2 if less flag		Set pc to imm op2 if less flag
	JLE,	//	Set pc to reg op2 if less eq flag	Set pc to imm op2 if less eq flag
	CALL,	//	Push pc to stack and set to reg op2	Push pc to stack and set to imm op2
	RET,	//	Pop top of stack and jump to it		Pop top of stack and jump to it
	INT,	//	Call from interrupt table. Arguments in stack, guaranteed to remain unchanged

	//	Arithmetic
	ADD,	//	Add reg op2 to rDest				Add imm op2 to rDest
	SUB,	//	Substract reg op2 from rDest		Substract imm op2 from rDest
	MUL,	//	Multiply reg op2 times rDest		Multiply imm op2 times rDest
	DIV,	//	Divide reg op2 by rDest				Divide imm op2 by rDest
	MOD,	//	Modulo of rDest by reg op2			Modulo of rDest by imm op2
	SHFT,	//	Shift rDest to the right by reg op2	Shift rDest to the right by imm op2

	//	Logic
	AND,	//	Bitwise AND of rDest and reg op2	Bitwise AND of rDest and imm op2
	NAND,	//	Bitwise NAND of rDest and reg op2	Bitwise NAND of rDest and imm op2
	OR,		//	Bitwise OR of rDest and reg op2		Bitwise OR of rDest and imm op2
	XOR,	//	Bitwise XOR of rDest and reg op2	Bitwise XOR of rDest and imm op2
	NOR,	//	Bitwise NOR of rDest and reg op2	Bitwise NOR of rDest and imm op2
	XNOR,	//	Bitwise XNOR of rDest and reg op2	Bitwise XNOR of rDest and imm op2
	NOT,	//	Bitwise neg. of reg op2 in rDest	Bitwise neg. of imm op2 in rDest
};

enum CompFlags
{
	equal,
	not_equal,
	greater,
	greater_equal,
	less,
	less_equal,

	total_flags
};

enum Reg
{
	ax,
	bx,
	cx,
	dx,
};

enum Interrupts
{
	// int name			args
	print_char,		//	ax: char
	new_line,		//	-
	read_disk,		//	ax: sector_start, bx: num_sectors, cx: mem_addr
	write_disk,		//	ax: mem_start, bx: num_sectors, cx: sector_addr

	total_interrupts
};

void MOV(std::ofstream* out, std::string instr)
{

}

void LD(std::ofstream* out, std::string instr)
{

}

void ST(std::ofstream* out, std::string instr)
{

}

void PUSH(std::ofstream* out, std::string instr)
{

}

void POP(std::ofstream* out, std::string instr)
{

}

void IN(std::ofstream* out, std::string instr)
{

}

void OUT(std::ofstream* out, std::string instr)
{

}

void HALT(std::ofstream* out, std::string instr)
{

}

void CMP(std::ofstream* out, std::string instr)
{

}

void JMP(std::ofstream* out, std::string instr)
{

}

void JEQ(std::ofstream* out, std::string instr)
{

}

void JNE(std::ofstream* out, std::string instr)
{

}

void JGT(std::ofstream* out, std::string instr)
{

}

void JGE(std::ofstream* out, std::string instr)
{

}

void JLT(std::ofstream* out, std::string instr)
{

}

void JLE(std::ofstream* out, std::string instr)
{

}

void CALL(std::ofstream* out, std::string instr)
{

}

void RET(std::ofstream* out, std::string instr)
{

}

void INT(std::ofstream* out, std::string instr)
{

}

void ADD(std::ofstream* out, std::string instr)
{

}

void SUB(std::ofstream* out, std::string instr)
{

}

void MUL(std::ofstream* out, std::string instr)
{

}

void DIV(std::ofstream* out, std::string instr)
{

}

void MOD(std::ofstream* out, std::string instr)
{

}

void SHFT(std::ofstream* out, std::string instr)
{

}

void AND(std::ofstream* out, std::string instr)
{

}

void NAND(std::ofstream* out, std::string instr)
{

}

void OR(std::ofstream* out, std::string instr)
{

}

void XOR(std::ofstream* out, std::string instr)
{

}

void NOR(std::ofstream* out, std::string instr)
{

}

void XNOR(std::ofstream* out, std::string instr)
{

}

void NOT(std::ofstream* out, std::string instr)
{

}

int main(int argc, char* argv[])
{
	opcodeFunc opcodes[32]
	{
		MOV,
		LD,
		ST,
		PUSH,
		POP,
		IN,
		OUT,
		HALT,
		CMP,
		JMP,
		JEQ,
		JNE,
		JGT,
		JGE,
		JLT,
		JLE,
		CALL,
		RET,
		INT,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		SHFT,
		AND,
		NAND,
		OR,
		XOR,
		NOR,
		XNOR,
		NOT
	};

	std::string path{};

	if (argc == 2)
		for (int i{}; argv[1][i] != '\0'; i++)
			path.push_back(argv[1][i]);

	std::ifstream input{ path };
	std::string fileName{ path.substr(path.find_last_of('\\') + 1).substr(0, path.find('.')) };
	std::ofstream output{ fileName + ".bin" };

	std::vector<std::string> content{};

	while (!input.eof())
	{
		char c{};
		input.read(&c, 1);
		if (c == '\n')
			content.push_back("");
		else
			content.back().push_back(c);
	}

	for (std::string& instr : content)
	{
		Opcode opcode
	}

	return 0;
}