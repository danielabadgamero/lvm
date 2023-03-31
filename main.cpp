#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "Opcodes.h"
#include "Regs.h"

struct Label
{
	std::string id{};
	int addr{};
};

std::vector<std::string> getContent(std::ifstream& input)
{
	std::vector<std::string> content(1);
	while (!input.eof())
	{
		char c{};
		input.read(&c, 1);
		if (c == '\n')
			content.push_back("");
		else if (c == '\t')
			content.back().push_back(' ');
		else
			content.back().push_back(c);
	}
	return content;
}

int main(int argc, char* argv[])
{
	std::string path{};
	if (argc == 2)
		path = argv[1];
	else
		std::cerr << "No input file.\n";
	
	std::ifstream input{ path };
	std::string fileName{ path.substr(path.find_last_of('\\') + 1).substr(0, path.find('.')) };
	std::ofstream output{ fileName + ".bin" };

	std::vector<std::string> content{ getContent(input) };

	std::vector<char> out{};
	std::vector<Label> labelDefinitions{};
	std::vector<Label> labelReferences{};

	int pc{};

	for (std::string& line : content)
	{
		std::vector<std::string> args{};
		for (char c : line)

	}

	return 0;
}