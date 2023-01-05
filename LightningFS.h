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
		* 16 bytes: directory or file name
		* if file: variable size of bytes until ASCII EOT (4) reached
		* 4 bytes: index of parent directory. EXCEPT directory /.
		* variable size and multiple of 4: index of subdirectories
		* variable size and multiple of 4: index of subfolders
		* 
		*/

		inline std::vector<unsigned char> filesystem{};
	}
}

#endif