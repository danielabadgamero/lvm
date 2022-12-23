#include <string>
#include <vector>
#include <deque>
#include <stdexcept>
#include <iostream>

#include "LightningLL.h"
#include "LightningCore.h"
#include "LightningOP.h"

static void writeBinary(Lightning::OP::Opcode opcode, std::string Rd, std::string Rs1, std::string Rs2, std::string imm)
{
	Lightning::LL::bin->push_back(std::to_string(opcode) + ' ' + Rd + ' ' + Rs1 + ' ' + Rs2 + ' ' + imm);
}

void Lightning::LL::compile(std::vector<std::string>* source, std::vector<std::string>* binary)
{
	bin = binary;
	bin->insert(bin->begin(), "%%");
	for (std::string line : *source)
	{
		std::string function{ line.substr(0, line.find(' ')) };
		args = line.substr(line.find(' ') + 1);
		functions[function]();
	}
}

void Lightning::LL::loadFunctions()
{
	functions["alloc"] = []()
	{
		bin->insert(bin->begin(), args);
	};
	
	functions["print"] = []()
	{
		if (!args.empty())
			if (args.front() == '"' && args.back() == '"')
			{
				bool escape{ false };
				for (int i{ 1 }; i != args.size() - 1; i++)
				{
					if (escape)
					{
						switch (args[i])
						{
						case 'n':
							args[i] = '\n';
							break;
						}
						escape = false;
					}
					if (args[i] == '\\' && escape == false)
						escape = true;
					if (!escape)
					{
						writeBinary
						(
							OP::SET,
							std::to_string(VAL),
							"0",
							"0",
							std::to_string(args[i])
						);
						writeBinary
						(
							OP::COUT,
							"0",
							std::to_string(VAL),
							"0",
							"0"
						);
					}
				}
			}
			else
			{
				writeBinary
				(
					OP::SET,
					std::to_string(ADDR),
					"0",
					"0",
					args
				);
				writeBinary
				(
					OP::RMEM,
					std::to_string(VAL),
					"0",
					std::to_string(ADDR),
					"0"
				);
				writeBinary
				(
					OP::IOUT,
					"0",
					std::to_string(VAL),
					"0",
					"0"
				);
			}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for print function.\n";
			return;
		}
	};

	functions["set"] = []()
	{
		if (!args.empty())
		{
			std::string symbol{ args.substr(0, args.find(' ')) };
			std::string val{ args.substr(args.find(' ') + 1) };
			try
			{
				int n{ std::stoi(val) };
				writeBinary
				(
					OP::SET,
					std::to_string(VAL),
					"0",
					"0",
					std::to_string(n)
				);
				writeBinary
				(
					OP::SET,
					std::to_string(ADDR),
					"0",
					"0",
					symbol
				);
				writeBinary
				(
					OP::WMEM,
					"0",
					std::to_string(VAL),
					std::to_string(ADDR),
					"0"
				);
			}
			catch (std::invalid_argument)
			{
				writeBinary
				(
					OP::SET,
					std::to_string(ADDR),
					"0",
					"0",
					val
				);
				writeBinary
				(
					OP::RMEM,
					std::to_string(VAL),
					"0",
					std::to_string(ADDR),
					"0"
				);
				writeBinary
				(
					OP::SET,
					std::to_string(ADDR),
					"0",
					"0",
					symbol
				);
				writeBinary
				(
					OP::WMEM,
					"0",
					std::to_string(VAL),
					std::to_string(ADDR),
					"0"
				);
			}
		}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for set function.\n";
			return;
		}
	};

	functions["halt"] = []()
	{
		writeBinary
		(
			OP::HALT,
			"0",
			"0",
			"0",
			"0"
		);
	};
}