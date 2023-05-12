#ifndef COMMANDS_H
#define COMMANDS_H

#include <unordered_map>
#include <string>

typedef void(*CommandFunc)(std::unordered_map<std::string, std::string>&);

namespace Commands
{
	inline std::unordered_map<std::string, CommandFunc> functions{};

	void init();
}

#endif