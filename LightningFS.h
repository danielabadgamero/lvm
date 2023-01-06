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
		*		variable size and multiple of 4: index of subfiles
		* 1 byte: ASCII 28 (FS)
		* 
		*/

		inline std::vector<std::vector<unsigned char>> filesystem
		{
			{
				1, 29, 'c', 'o', 'r', 'e', 29,
				CPU::SET, AR, 0, 0,		// address 0 of filesystem
				CPU::SET, R7, 0, 1,		// restart address
				CPU::SET, LR, 0, 4,		// start of loop address

				CPU::JPI, 0, 0, 6,		// skip function definition
				CPU::SET, R0, 0, 28,	// entry separator
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 2,	// skip recursivity if new file
				CPU::CALL, 0, LR, 0,	// recursive call
				CPU::RET, 0, 0, 0,		// return

				CPU::JPI, 0, 0, 5,		// skip function definition
				CPU::RFS, 0, 0, 1,		// read next byte of FS
				CPU::SEQ, TR, DR, R0,	// test byte
				CPU::INC, AR, 0, 1,		// jump to next byte
				CPU::RET, 0, 0, 0,		// return

				CPU::SET, R1, 0, 10,	// set address of check function

				CPU::SET, R0, 0, 0,		// check for directory
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 2,	// if a directory, skip function call
				CPU::CALL, 0, LR, 0,	// call function to go to next entry

				CPU::INC, AR, 0, 1,		// jump to folder name

				CPU::SET, R0, 0, 's',	// check directory name "sys"
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 3,	// correct character for name
				CPU::CALL, 0, LR, 0,	// wrong character name
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'y',	// check directory name "sys"
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 3,	// correct character for name
				CPU::CALL, 0, LR, 0,	// wrong character name
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 's',	// check directory name "sys"
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 3,	// correct character for name
				CPU::CALL, 0, LR, 0,	// wrong character name
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 28,	// check directory name "sys"
				CPU::INC, R0, 0, 1,		// adjust to 29
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 3,	// correct character for name
				CPU::CALL, 0, LR, 0,	// wrong character name
				CPU::JMP, 0, R7, 0,		// restart

				CPU::OUTI, 'O', 'p', 'e',
				CPU::OUTI, 'r', 'a', 't',
				CPU::OUTI, 'i', 'n', 'g',
				CPU::OUTI, ' ', 's', 'y',
				CPU::OUTI, 's', 't', 'e',
				CPU::OUTI, 'm', ' ', 'f',
				CPU::OUTI, 'o', 'u', 'n',
				CPU::OUTI, 'd', ' ', 'a',
				CPU::OUTI, 't', ' ', 'a',
				CPU::OUTI, 'd', 'd', 'r',
				CPU::OUTI, 'e', 's', 's',
				CPU::OUTI, ':', ' ', 0,

				CPU::SET, R6, 0, 8,		// for shift operations
				CPU::SET, R3, 0, 0,		// for final address

				CPU::INC, AR, 0, 6,		// jump to directory subfiles
				CPU::RFS, 0, 0, 1,		// read first byte of address
				CPU::ADD, R3, DR, R3,	// build final address

				CPU::INC, AR, 0, 1,		// jump to next byte
				CPU::RFS, 0, 0, 1,		// read first byte of address
				CPU::LSFT, R3, R3, R6,	// adjust for final address
				CPU::ADD, R3, DR, R3,	// build final address

				CPU::INC, AR, 0, 1,		// jump to next byte
				CPU::RFS, 0, 0, 1,		// read first byte of address
				CPU::LSFT, R3, R3, R6,	// adjust for final address
				CPU::ADD, R3, DR, R3,	// build final address

				CPU::INC, AR, 0, 1,		// jump to next byte
				CPU::RFS, 0, 0, 1,		// read first byte of address
				CPU::LSFT, R3, R3, R6,	// adjust for final address
				CPU::ADD, R3, DR, R3,	// build final address

				CPU::CPY, R5, R3, 0,	// save for later use

				CPU::SET, TR, 0, 10,	// to print as decimal
				CPU::CPY, R4, R3, 0,	// copy the value to divide
				CPU::SET, TR, 0, 10,	// set to divide and substract
				CPU::DIV, R4, R4, TR,	// get left digit
				CPU::INC, R4, 0, 48,	// ASCII value
				CPU::OUT, 0, R4, 0,		// print first digit
				CPU::INC, R4, 0, (unsigned char)-48,
				CPU::MUL, TR, TR, R4,	// to substract from initial value
				CPU::SUB, R3, R3, TR,	// get right digit
				CPU::INC, R3, 0, 48,	// ASCII value
				CPU::OUT, 0, R3, 0,		// and print it

				CPU::OUTI, 0, 0, '\n',

				CPU::CPY, AR, R5, 0,	// load the address of the kernel
				CPU::SET, R0, 0, 1,		// check entry is a file
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				29, 0, 0, 0, 0, 28
			},
			{
				0, 29, 's', 'y', 's',
				29, 0, 0, 0, 0, 29, 29,
				0, 0, 0, 17, 28,
				1, 29, 
			}
		};
	}
}

#endif