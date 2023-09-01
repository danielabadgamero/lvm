#ifndef ASM_H
#define ASM_H

#include <string>
#include <filesystem>
#include <unordered_map>

namespace Asm
{
	inline std::unordered_map<std::string, int> command_def{};
	inline std::unordered_map<std::string, unsigned short> sym_def{};
	inline std::unordered_map<unsigned short, std::string> sym_ref{};

	void assemble(const std::filesystem::path&);
}

#endif
