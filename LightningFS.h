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

		inline std::vector<std::vector<unsigned char>> filesystem
		{
			{
				1, 29, 'c', 'o', 'r', 'e', 29,
				CPU::SET, R0, 0, 2,
				CPU::SET, R1, 0, '\r',
				CPU::COUTI, '\n', '>', ' ',
				CPU::CIN, R2, 0, 0,
				CPU::COUT, 0, R2, 0,
				CPU::SEQ, TR, R2, R1,
				CPU::JNZ, 0, R0, TR,
				CPU::JPIB, 0, 0, 4,
				29, 0, 28
			},
			{}
		};
	}
}

#endif