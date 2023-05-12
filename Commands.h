#ifndef COMMANDS_H
#define COMMANDS_H

#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, std::string>& Args;

struct CommandFunc
{
	void(*func)(Args) {};
	std::vector<std::string> reqArgs{};
	std::vector<std::string> optArgs{};

	CommandFunc() {};
	CommandFunc(void(*func)(Args), std::vector<std::string> = {}, std::vector<std::string> = {});
	void operator()(Args);
};

namespace Commands
{
	inline std::unordered_map<std::string, CommandFunc> functions{};

	void init();
}

#endif