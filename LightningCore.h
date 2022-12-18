#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <functional>
#include <stack>

/*

LightLang
=========

1. All programmes must start with "LightLand v1.0".
2. The keyword "alloc" will indicate memory must be allocated for the piece of data in the same line.
3. Variables and functions are the same: memory addresses. Variables return immediately a value, whereas functions go through a process.
4. Functions are defined with the keywords "begin" (after the function name) and ended with "end" (on a newline).
5. There are no data types.

Syntax:
-------

- Variable declaration:
	alloc <name> <value>
Note: initial value is required and cannot be another variable. One step at a time, please.

- Function declaration:
	alloc <name> begin
	<instructions>
	end

- Change value of variable:
	set <name> <new value>
Note: <new value> may be another variable or function.

- Variable and function calling:
	<name> <params...>

Pre-defined functions:
----------------------

halt:
	Terminates the programme and frees up all resources.

add <name> <name>:
	Returns sum of both parameters.

mult <name> <name>:
	Returns product of both parameters.

div <name> <name>:
	Returns integer quotient of both parameters.

mod <name> <name>:
	Returns the remainder of the division of both parameters.

out <name> <type>:
	Prints the parameter to the console according to the type specified.

in <name>:
	Gets a value from the user and sets it to the parameter.

*/

/*

opcodes:

halt:
	Termiante the programme and free all allocated memory.

set <a> <b>:
	Set cell at address <a> to <b>.

wmem <a> <b>:
	Set cell at address <a> to cell at address <b>.

cout <a> <b>:
	Print value at memory address <a> as character if b is 1, as an integer otherwise.

call <a>:
	Push next instruction's address after a to stack and jump to memory address at <a>.
*/

namespace Lightning
{
	enum class Opcode
	{
		HALT,
		SET,
		WMEM,
		ADD,
		COUT,
		CALL,
		RET,
	};

	enum class Mode
	{
		CMD,
		TEXT,
		EXEC,
	};

	struct Cell
	{
		int value{};
		bool allocated{};
	};

	struct Symbol
	{
		int add{};
		int val{};
	};

	struct Function
	{
		int add{};
		std::vector<int> args{};
		int ret{};
	};

	struct Programme
	{
		std::string name{};
		int add{};
	};

	inline std::map<std::string, Function> functions
	{
		{ "cout", {} },
	};

	inline std::vector<Programme> loadedProgrammes{};

	inline Cell RAM[1 << 15];
	inline Mode mode{ Mode::CMD };
	inline std::stack<int> stack{};

	inline Cell* addr{ RAM };
	inline bool running{ true };

	void init();
	void clearScreen();
	void loadProgramme();
}

#endif