#ifndef ASM_H
#define ASM_H

#include <string>
#include <filesystem>
#include <unordered_map>

namespace Asm
{
	inline std::unordered_map<std::string, unsigned short> command_def{};
	inline std::unordered_map<std::string, unsigned short> global_def{};
	inline std::unordered_map<unsigned short, std::string> global_ref{};

	inline unsigned short pc{};

	void assemble(const std::filesystem::path&);
}

#endif
