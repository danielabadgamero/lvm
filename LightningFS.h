#ifndef LIGHTNING_FS
#define LIGHTNING_FS

#include <vector>

namespace Lightning
{
	namespace FS
	{
		/*
		* Filesystem structure:
		* 
		* 1 byte: 0 for directory, 1 for file
		* 1 byte: ASCII 29 (GS)
		* variable size of bytes: name
		* 1 byte: ASCII 29 (GS)
		* if file:
		*		variable size of bytes: file content
		*		1 byte: ASCII 29 (GS)
		* 4 bytes: index of parent directory. EXCEPT directory /.
		* if directory:
		*		1 byte: ASCII 29 (GS)
		*		variable size and multiple of 4: index of subdirectories
		*		1 byte: ASCII 29 (GS)
		*		variable size and multiple of 4: index of subfolders
		* 1 byte: ASCII 28 (FS)
		* 
		*/

		inline std::vector<unsigned char> filesystem
		{
			1, 29, 'c', 'o', 'a', 'e', 29,
			CPU::SET, R0, 0, 'H',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'e',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'l',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'l',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'o',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, ',',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, ' ',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'w',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'o',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'r',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'l',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, 'd',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, '!',
			CPU::COUT, 0, R0, 0,
			CPU::SET, R0, 0, '\n',
			CPU::COUT, 0, R0, 0,
			29, 0, 28
		};
	}
}

#endif