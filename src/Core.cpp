#include "Core.h"
#include "Asm.h"
#include "VM.h"

#include <sstream>
#include <iostream>
#include <filesystem>

void Core::init()
{
	commands["help"] = 
		[](const std::string& cmd)
		{
			if (cmd.empty()) for (const auto& c : commands)
				std::cout << c.first << std::endl;
			else if (desc.contains(cmd)) std::cout << cmd << ": " << desc.at(cmd) << std::endl;
			else std::cout << "Command not found" << std::endl;
		};
	desc["help"] = "Display a list of all commands and get specific information about one";

	commands["exit"] = 
		[](const std::string&)
		{
			running = false;
		};
	desc["exit"] = "Stop everything and quit";

	commands["list"] = 
		[](const std::string&)
		{
			for (const std::filesystem::directory_entry& d :
					std::filesystem::directory_iterator("./prog"))
				std::cout << d.path().filename().string() << std::endl;
		};
	desc["list"] = "Display a list of all stored programmes";

	commands["build"] = 
		[](const std::string& prog)
		{
			const std::filesystem::path prog_path{ "./prog/" + prog };
			if (!std::filesystem::exists(prog_path))
			{
				std::cout << "Programme does not exist" << std::endl;
				return;
			}
			Asm::assemble(prog_path);
		};
	desc["build"] = "Build a programme with assembly input and binary output";
	
	commands["run"] =
		[](const std::string& prog)
		{
			VM::commands.clear();
			const std::filesystem::path prog_path{ "./prog/" + prog };
			if (!std::filesystem::exists(prog_path))
			{
				std::cout << "Programme does not exist" << std::endl;
				return;
			}
			VM::loadCommands(prog_path);
		};
	desc["run"] = "Initialize a programme to handle further commands until it terminates";
}

void Core::loop()
{
	while (running)
	{
		std::cout << "> ";
		std::string input{};
		std::getline(std::cin, input);
		std::stringstream line{ input };
		std::string cmd{};
		std::string arg{};
		std::getline(line, cmd, ' ');
		std::getline(line, arg, ' ');
		
		if (executing)
		{
			if (VM::commands.contains(cmd)) VM::execute(cmd);
			else std::cout << "Unregistered command" << std::endl;
		}
		else
		{
			if (commands.contains(cmd)) commands.at(cmd)(arg);
			else std::cout << "Unknown command" << std::endl;
		}
	}
}

void Core::quit()
{
}
