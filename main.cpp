#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
	std::string path{};

	if (argc == 2)
		for (int i{}; argv[1][i] != '\0'; i++)
			path.push_back(argv[1][i]);

	std::ifstream input{ path };
	std::string fileName{ path.substr(path.find_last_of('\\') + 1).substr(0, path.find('.')) };
	std::ofstream output{ fileName + ".bin" };

	std::vector<std::string> content{};

	while (!input.eof())
	{
		char c{};
		input.read(&c, 1);
		if (c == '\n')
			content.push_back("");
		else
			content.back().push_back(c);
	}

	for (std::string& instr : content)
		if (instr == "")

	return 0;
}