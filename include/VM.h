#ifndef VM_H
#define VM_H

#include <stack>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace VM
{
	inline std::unordered_map<std::string, unsigned short> commands{};

	enum Opcode
	{
		//	arg1	arg2
		HLT,//	----	----
		CAL,//	dest	----
		RET,//	----	----
		PSH,//	srce	----
		POP,//	dest	----
		SIG,//	dest	----
		MOV,//	dest	srce
		CMP,//	dest	srce
		JMP,//	dest	----
		ADD,//	dest	srce
		SUB,//	dest	srce
		MUL,//	dest	srce
		DIV,//	dest	srce
		MOD,//	dest	srce
		AND,//	dest	srce
		NOT,//	dest	----
	};

	inline std::unordered_map<std::string, unsigned char> opcodes
	{
		{ "HLT", 0 },
		{ "CAL", 1 },
		{ "RET", 2 },
		{ "PSH", 3 },
		{ "POP", 4 },
		{ "SIG", 5 },
		{ "MOV", 6 },
		{ "CMP", 7 },
		{ "JMP", 8 },
		{ "ADD", 9 },
		{ "SUB", 10 },
		{ "MUL", 11 },
		{ "DIV", 12 },
		{ "MOD", 13 },
		{ "AND", 14 },
		{ "NOT", 15 },
	};

	enum Flag
	{
		always_true,
		equal,
		not_equal,
		zero,
		greater,
		greater_equal,
		lower,
		lower_equal,
	};

	inline unsigned short pc{};
	inline unsigned char RAM[1 << 16]{};
	inline unsigned char disk[1 << 16]{};
	inline bool running{};
	inline bool flags[8]{};
	inline std::filesystem::path prog_path{};
	inline std::string prog_name{};
	inline std::stack<unsigned short> stack{};
	inline std::stack<unsigned short> call_stack{};

	void loadCommands(const std::filesystem::path&);
	void execute(const std::string&);
}

#endif
