#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include "Core.h"
#include "Errors.h"
#include "Commands.h"
#include "Text.h"
#include "Lightning.h"
#include "Assembler.h"

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

	functions["mkdir"] = CommandFunc
	{
		[](Args args)
		{
			std::filesystem::create_directory(args["default"]);
		},
		{ "default" }
	};

	functions["rmdir"] = CommandFunc
	{
		[](Args args)
		{
			std::filesystem::remove_all(args["default"]);
		},
		{ "default" }
	};

	functions["mkfile"] = CommandFunc
	{
		[](Args args)
		{
			std::ofstream out{ args["default"] };
		},
		{ "default" }
	};

	functions["rmfile"] = CommandFunc
	{
		[](Args args)
		{
			std::filesystem::remove(args["default"]);
		},
		{ "default" }
	};

	functions["print"] = CommandFunc
	{
		[](Args args)
		{
			std::ifstream in{ args["default"] };
			std::string line{};
			while (std::getline(in, line))
				std::cout << line << '\n';
		},
		{ "default" }
	};

	functions["edit"] = CommandFunc
	{
		[](Args args)
		{
			if (!std::filesystem::exists(args["default"])) E6(args["default"]);

			std::ifstream in{ args["default"] };
			Core::mode = Core::TXT;
			Text::init(in, args["default"]);
		},
		{ "default" }
	};

	functions["help"] = CommandFunc
	{
		[](Args)
		{
			for (const std::pair<const std::string, CommandFunc>& pair : functions)
			{
				std::cout << pair.first << ' ';
				for (const std::string& arg : pair.second.optArgs)
					std::cout << arg << ' ';
				std::cout << '\n';
			}
		}
	};

	functions["exit"] = CommandFunc
	{
		[](Args)
		{
			std::remove("disk");
			std::ofstream diskOut{ "disk", std::ios::binary };
			for (int i{}; i != 1 << 16; i++)
				for (int j{}; j != 512; j++)
					diskOut.write(&Lightning::disk[i][j], 1);
			Core::running = false;
		}
	};

	functions["lasm"] = CommandFunc
	{
		[](Args args)
		{
			std::ifstream input{ args["default"] };
			std::string line{};
			std::vector<std::string> content{};
			while (std::getline(input, line))
				content.push_back(line);
			std::string fileName{ args["default"].substr(0, args["default"].find('.')) };
			std::ofstream output{ fileName + ".bin", std::ios::binary };
			std::vector<char> bytes{ Assembler::assemble(content) };
			output.write(bytes.data(), bytes.size());
		},
		{ "default" }
	};

	functions["start"] = CommandFunc
	{
		[](Args args)
		{
			std::ifstream in{ args["default"], std::ios::binary };
			Lightning::reset();
			in.read(Lightning::RAM, 1ll << 16);
			Core::mode = Core::EXE;
		},
		{ "default" }
	};

	if (std::filesystem::exists("disk"))
	{
		std::ifstream diskIn{ "disk", std::ios::binary };
		char c{};
		int i{};
		int j{};
		diskIn.read(&c, 1);
		while (!diskIn.eof())
		{
			Lightning::disk[i][j] = c;
			if (i == 1 << 16) i = 0;
			if (j == 512) j = 0;
			diskIn.read(&c, 1);
		}
	}
}