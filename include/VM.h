#ifndef VM_H
#define VM_H

#include <string>
#include <filesystem>
#include <unordered_map>

namespace VM
{
	inline std::unordered_map<std::string, unsigned short> commands{};

	enum Opcode
	{
		//	arg1	arg2	arg3
		HLT,//	----	----	----
		RET,//	----	----	----
		SND,//	----	----	----
		STO,//	dest	srce	----
		MOV,//	dest	srce	----
		JMP,//	dest	----	----
		JGT,//	dest	src1	src2
		JEQ,//	dest	src1	src2
		ADD,//	dest	srce	----
		SUB,//	dest	srce	----
		MUL,//	dest	srce	----
		DIV,//	dest	srce	----
		MOD,//	dest	srce	----
		AND,//	dest	srce	----
		NOR,//	dest	srce	----
		NOT,//	dest	----	----
	};

	inline unsigned short pc{};
	inline unsigned char RAM[1 << 16]{};
	inline unsigned char disk[1 << 16]{};
	inline bool running{};
	inline std::filesystem::path prog_path{};
	inline std::string prog_name{};

	void loadCommands(const std::filesystem::path&);
	void execute(const std::string&);
}

#endif
