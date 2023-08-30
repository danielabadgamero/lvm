#include "Asm.h"

#include <fstream>
#include <iostream>

void Asm::assemble(const std::filesystem::path& prog_path)
{
	std::string prog_name{ prog_path.filename() };
	std::filesystem::path bin_path{ prog_path / (prog_name + ".asm") };
	if (!std::filesystem::exists(bin_path))
	{
		std::cout << "Assembly file not found" << std::endl;
		return;
	}
	std::ifstream bin{ bin_path.string(), std::ios::binary };

	char byte{};
	bin.read(&byte, 1);
	while (!bin.eof())
	{
			

		bin.read(&byte, 1);
	}
}
