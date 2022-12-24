#include <string>
#include <vector>
#include <deque>
#include <stdexcept>
#include <iostream>

#include "LightningLL.h"
#include "LightningCore.h"
#include "LightningOP.h"

static void writeBin(Lightning::OP::Opcode opcode, int Rd, int Rs1, int Rs2, std::string imm = "0")
{
	Lightning::LL::bin->push_back(std::to_string(opcode) + ' ' + std::to_string(Rd) + ' ' + std::to_string(Rs1) + ' ' + std::to_string(Rs2) + ' ' + imm);
}

void Lightning::LL::compile(std::vector<std::string>* source, std::vector<std::string>* binary)
{
	bin = binary;
	bin->insert(bin->begin(), "%%");
	for (std::vector<std::string>::iterator line{ source->begin() }; line != source->end(); line++)
	{
		std::string function{ line->substr(0, line->find(' ')) };
		args = line->substr(line->find(' ') + 1);
		keywords[function]();
	}
}

void Lightning::LL::loadFunctions()
{
	keywords["alloc"] = []()
	{
		bin->insert(bin->begin(), args);
	};
	
	keywords["print"] = []()
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
						writeBin(OP::SET, VAL1, 0, 0, std::to_string(args[i]));
						writeBin(OP::COUT, 0, VAL1, 0);
					}
				}
			}
			else
			{
				writeBin(OP::SET, ADDR, 0, 0, args);
				writeBin(OP::RMEM, VAL1, 0, ADDR);
				writeBin(OP::IOUT, 0, VAL1, 0);
			}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for print function.\n";
			return;
		}
	};

	keywords["println"] = []()
	{
		if (!args.empty())
		{
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
						writeBin(OP::SET, VAL1, 0, 0, std::to_string(args[i]));
						writeBin(OP::COUT, 0, VAL1, 0);
					}
				}
			}
			else
			{
				writeBin(OP::SET, ADDR, 0, 0, args);
				writeBin(OP::RMEM, VAL1, 0, ADDR);
				writeBin(OP::IOUT, 0, VAL1, 0);
			}
			writeBin(OP::SET, VAL1, 0, 0, std::to_string('\n'));
			writeBin(OP::COUT, 0, VAL1, 0);
		}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for print function.\n";
			return;
		}
	};

	keywords["set"] = []()
	{
		if (!args.empty())
		{
			std::string symbol{ args.substr(0, args.find(' ')) };
			std::string val{ args.substr(args.find(' ') + 1) };
			try
			{
				int n{ std::stoi(val) };
				writeBin(OP::SET, VAL1, 0, 0, std::to_string(n));
				writeBin(OP::SET, ADDR, 0, 0, symbol);
				writeBin(OP::WMEM, 0, VAL1, ADDR);
			}
			catch (std::invalid_argument)
			{
				writeBin(OP::SET, ADDR, 0, 0, val);
				writeBin(OP::RMEM, VAL1, 0, ADDR);
				writeBin(OP::SET, ADDR, 0, 0, symbol);
				writeBin(OP::WMEM, 0, VAL1, ADDR);
			}
		}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for set function.\n";
			return;
		}
	};

	keywords["add"] = []()
	{
		if (!args.empty())
		{
			std::string symbol{ args.substr(0, args.find(' ')) };
			std::string val{ args.substr(args.find(' ') + 1) };
			try
			{
				int n{ std::stoi(val) };
				writeBin(OP::SET, VAL1, 0, 0, std::to_string(n));
				writeBin(OP::SET, ADDR, 0, 0, symbol);
				writeBin(OP::RMEM, VAL2, 0, ADDR);
				writeBin(OP::ADD, VAL1, VAL1, VAL2);
				writeBin(OP::WMEM, 0, VAL1, ADDR);
			}
			catch (std::invalid_argument)
			{
				writeBin(OP::SET, ADDR, 0, 0, val);
				writeBin(OP::RMEM, VAL1, 0, ADDR);
				writeBin(OP::SET, ADDR, 0, 0, symbol);
				writeBin(OP::RMEM, VAL2, 0, ADDR);
				writeBin(OP::ADD, VAL1, VAL1, VAL2);
				writeBin(OP::WMEM, 0, VAL1, ADDR);
			}
		}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for add function.\n";
		}
	};

	keywords["mul"] = []()
	{
		if (!args.empty())
		{
			std::string symbol{ args.substr(0, args.find(' ')) };
			std::string val{ args.substr(args.find(' ') + 1) };
			try
			{
				int n{ std::stoi(val) };
				writeBin(OP::SET, VAL1, 0, 0, std::to_string(n));
				writeBin(OP::SET, ADDR, 0, 0, symbol);
				writeBin(OP::RMEM, VAL2, 0, ADDR);
				writeBin(OP::MUL, VAL1, VAL1, VAL2);
				writeBin(OP::WMEM, 0, VAL1, ADDR);
			}
			catch (std::invalid_argument)
			{
				writeBin(OP::SET, ADDR, 0, 0, val);
				writeBin(OP::RMEM, VAL1, 0, ADDR);
				writeBin(OP::SET, ADDR, 0, 0, symbol);
				writeBin(OP::RMEM, VAL2, 0, ADDR);
				writeBin(OP::MUL, VAL1, VAL1, VAL2);
				writeBin(OP::WMEM, 0, VAL1, ADDR);
			}
		}
		else
		{
			std::cout << "Compilation failed: " << args << "no arguments provided for mul function.\n";
		}
	};

	keywords["halt"] = []()
	{
		writeBin(OP::HALT, 0, 0, 0);
	};
}