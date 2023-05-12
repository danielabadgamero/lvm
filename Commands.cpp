#include <iostream>
#include <string>
#include <unordered_map>

#include "Core.h"
#include "Commands.h"

void Commands::init()
{
	functions["echo"] = [](std::unordered_map<std::string, std::string>& args)
	{
		std::cout << args["default"];
	};

	functions["exit"] = [](std::unordered_map<std::string, std::string>&)
	{
		Core::running = false;
	};
}