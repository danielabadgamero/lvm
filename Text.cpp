#include <vector>
#include <string>
#include <iostream>

#include "Core.h"
#include "Text.h"

static void printFile()
{
	printf("\033c");
	for (int i{}; const std::string& line : Text::file)
		std::cout << ++i << "\t" << line << '\n';
	std::cout << '\n';
}

void Text::init(std::ifstream& in, std::string& fName)
{
	name = fName;
	std::string line{};
	file.clear();
	while (std::getline(in, line))
		file.push_back(line);
}

void Text::loop()
{
	printFile();

	std::string input{};
	std::getline(std::cin, input);

	int n{ static_cast<int>(file.size()) };
	try
	{
		n = std::stoi(input.substr(1, input.find(' ') - 1));
	}
	catch (...) {}


	switch (input[0])
	{
	case '+':
		if (n > file.size()) break;
		if (n < 1) break;
		file.insert(file.begin() + n, input.substr(input.find(' ') + 1));
		break;
	case '-':
		if (n > file.size()) break;
		if (n < 1) break;
		file.erase(file.begin() + n - 1);
		break;
	case '?':
		switch (input[1])
		{
		case 'q':
			Core::mode = Core::CMD;
			break;
		case 's':
			if (!file.empty())
			{
				std::ofstream out{ name };
				for (const std::string& line : file)
					out << line << '\n';
			}
			break;
		}
		break;
	case '\\':
		input.erase(input.begin());
	default:
		file.push_back(input);
	}
}