#ifndef ASM_H
#define ASM_H

#include <string>
#include <filesystem>
#include <unordered_map>

namespace Asm
{
	inline std::unordered_map<std::string, int> symbol_definitions{};
	inline std::unordered_map<std::string, int> symbol_references{};

	void assemble(const std::filesystem::path&);
}

#endif
