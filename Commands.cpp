#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "Core.h"
#include "Errors.h"
#include "Commands.h"

CommandFunc::CommandFunc(void(*func)(Args), std::vector<std::string> r, std::vector<std::string> o) : func{ func }, reqArgs{ r }, optArgs{ o }
{
	for (const std::string& arg : reqArgs)
		optArgs.push_back(arg);
}

void CommandFunc::operator()(Args args)
{
	for (std::string& reqArg : reqArgs)
		if (!args.contains(reqArg))	E3(reqArg);
	
	for (std::pair<const std::string, std::string>& pair : args)
		if (std::find(optArgs.begin(), optArgs.end(), pair.first) == optArgs.end()) E4(pair.first);

	func(args);
}

void Commands::init()
{
	functions["echo"] = CommandFunc
	{
		[](Args args)
		{
			std::cout << args["default"];
		},
		{ "default" }
	};

	functions["ls"] = CommandFunc
	{
		[](Args)
		{
			std::cout << '\n';
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
				std::cout << entry.path().filename().string() << (entry.is_directory() ? '/' : '\0') << '\n';
		},
	};

	functions["cd"] = CommandFunc
	{
		[](Args args)
		{
			if (!std::filesystem::exists(args["default"])) E5(args["default"]);

			std::filesystem::current_path(std::filesystem::current_path().append(args["default"]));
		},
		{ "default" }
	};

	functions["exit"] = CommandFunc
	{
		[](Args)
		{
			Core::running = false;
		}
	};
}