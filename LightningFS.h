#ifndef LIGHTNING_FS
#define LIGHTNING_FS

#include "LightningCore.h"

namespace Lightning
{
	namespace FS
	{
		/*
		* Filesystem structure:
		* -32 bytes: file name
		* -variable size: file content
		* -1 byte: 26
		* 
		*/

		inline char filesystem[1 << 29] // 512MB HDD
		{
		};
	}
}

#endif