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
				// setup
				CPU::SET, AR, 0, 0,		// address 0 of filesystem
				CPU::SET, R7, 0, 1,		// restart address
				CPU::SET, LR, 0, 4,		// start of loop address

				// function to point to next file or directory in filesystem
				CPU::JPI, 0, 0, 6,		// skip function definition
				CPU::SET, R0, 0, 28,	// entry separator
				CPU::CALL, 0, R1, 0,	// call check function
				CPU::JNZI, 0, TR, 2,	// skip recursivity if new file
				CPU::CALL, 0, LR, 0,	// recursive call
				CPU::RET, 0, 0, 0,		// return

				// function to read a byte from filesystem and compare it with a parameter
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

				// info print: found os folder
				CPU::COUTI, 'F', 'o', 'u',
				CPU::COUTI, 'n', 'd', ' ',
				CPU::COUTI, '/', 's', 'y',
				CPU::COUTI, 's', ' ', 'd',
				CPU::COUTI, 'i', 'r', 'e',
				CPU::COUTI, 'c', 't', 'o',
				CPU::COUTI, 'r', 'y', '.',
				CPU::COUTI, '\n', 0, 0,

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

				CPU::SET, R1, 0, 10,	// set R1 to function
				CPU::CPY, AR, R3, 0,	// load the address of the kernel

				CPU::SET, R0, 0, 1,		// check entry is a file
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 28,	// check byte
				CPU::INC, R0, 0, 1,		// adjust
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'k',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'r',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'n',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'l',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart
				
				CPU::SET, R0, 0, '.',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart
				
				CPU::SET, R0, 0, 'e',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'x',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 'e',	// check byte
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				CPU::SET, R0, 0, 28,	// check byte
				CPU::INC, R0, 0, 1,		// adjust
				CPU::CALL, 0, R1, 0,	// call check character
				CPU::JNZI, 0, TR, 3,	// correct character
				CPU::CALL, 0, LR, 0,	// wrong character
				CPU::JMP, 0, R7, 0,		// restart

				// info print: kernel found
				CPU::COUTI, 'F', 'o', 'u',
				CPU::COUTI, 'n', 'd', ' ',
				CPU::COUTI, 'k', 'r', 'n',
				CPU::COUTI, 'l', '.', 'e',
				CPU::COUTI, 'x', 'e', '.',
				CPU::COUTI, '\n', 0, 0,

				CPU::SET, R2, 0, 0,		// RAM address
				CPU::CPY, R3, AR, 0,	// FS address
				CPU::SET, R0, 0, 28,	// byte 28
				CPU::INC, R0, 0, 1,		// adjust to 29

				CPU::CPY, AR, R3, 0,	// load FS address
				CPU::CALL, 0, R1, 0,	// read content
				CPU::CPY, AR, R2, 0,	// load RAM address
				CPU::JNZI, 0, TR, 5,	// exit loop if byte is 29
				CPU::WMEM, 0, 0, 0,		// write byte
				CPU::INC, R2, 0, 1,		// jump to next address
				CPU::INC, R3, 0, 1,		// jump to next byte
				CPU::JPI, 0, 0, (unsigned char)-7,

				// info print: kernel written to RAM
				CPU::COUTI, 'D', 'o', 'n',
				CPU::COUTI, 'e', ' ', 'w',
				CPU::COUTI, 'r', 'i', 't',
				CPU::COUTI, 'i', 'n', 'g',
				CPU::COUTI, ' ', 'k', 'r',
				CPU::COUTI, 'n', 'l', '.',
				CPU::COUTI, 'e', 'x', 'e',
				CPU::COUTI, ' ', 't', 'o',
				CPU::COUTI, ' ', 'm', 'e',
				CPU::COUTI, 'm', 'o', 'r',
				CPU::COUTI, 'y', '.', '\n',
				
				// self destruct sequence
				CPU::SET, DR, 0, 0,		// byte to overwrite RAM
				CPU::WMEM, AR, 0, 1,	// erase byte
				CPU::INC, AR, 0, 1,		// next byte
				CPU::JPI, 0, 0, (unsigned char)-2,

				29, 0, 0, 0, 0, 28
			},
			{
				0, 29, 's', 'y', 's', 29,
				0, 0, 0, 0, 29,
				29,
				0, 0, 0, 17, 28,

				1, 29, 'k', 'r', 'n', 'l', '.', 'e', 'x', 'e', 29,

				

				29, 0, 0, 0, 0, 28
			}
		};
	}
}

#endif