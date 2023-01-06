#ifndef LIGHTNING_FS
#define LIGHTNING_FS

#include "LightningCore.h"

namespace Lightning
{
	namespace FS
	{
		/*
		* Filesystem structure:
		* 1 byte: file extension / 0 for file, 1 byte: pointer to parent directory (except root)
		* 8 bytes: 
		* 
		*/

		inline int filesystem[1 << 29] // 2GB HDD
		{
			1,					// root directory


			1, 0,
		};
	}
}

#endif