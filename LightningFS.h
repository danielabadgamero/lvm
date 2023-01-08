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
			'c', 'o', 'r', 'e', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			CPU::SET, R0, 0, 23,
			CPU::SET, AR, 16, 0,
			CPU::SET, CT, 0, '\r',
			CPU::SET, LR, 0, '\b',

			CPU::COUTI, '>', ' ', 0,
			CPU::CIN, DR, 0, 0,
			CPU::SEQ, TR, DR, CT,
			CPU::JPZI, 0, TR, 4,
			CPU::COUTI, '\n', '\r', 0,
			CPU::CALL, 0, R0, 0,
			CPU::JPI, 0, 0, -6,
			CPU::SEQ, TR, DR, LR,
			CPU::JPZI, 0, TR, 7,
			CPU::SET, R0, 16, 0,
			CPU::SGT, TR, AR, R0,
			CPU::JPZI, 0, TR, 4,
			CPU::SET, DR, 0, 0,
			CPU::INC, AR, 0, -1,
			CPU::WMEM, 0, 0, 0,
			CPU::COUT, 0, DR, 0,
			CPU::WMEM, 0, 0, 0,
			CPU::INC, AR, 0, 1,
			CPU::JPI, 0, 0, -17,

			CPU::RET, 0, 0, 0,
			26,
		};
	}
}

#endif