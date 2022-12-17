#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>

typedef std::map<std::string, std::string>* Arguments;

namespace Lightning
{
	enum class Opcode
	{
		HALT,
	};

	enum class Mode
	{
		CMD,
		TEXT,
	};
	
	struct Dir
	{
		struct File
		{
			std::string name{};
			std::string content{};
		};

		std::string name{};
		std::vector<File> files{};
		std::vector<Dir*> subDirs{};
	};

	inline int RAM[1 << 15];
	inline int REG[8];
	inline std::ifstream fs_in{};
	inline std::ofstream fs_out{};
	inline Dir Filesystem{};
	inline std::vector<Dir*> path{ &Filesystem };
	inline Dir::File* targetFile{ nullptr };
	inline Mode mode{ Mode::CMD };
	inline std::vector<std::string> fileContent{};

	inline int* addr{ RAM };
	inline bool running{ true };

	void loadFilesystem();
	void writeFilesystem(Dir* dir);
	void saveFilesystem();
	std::string getPath();
	void printPath();
	void printFileContent();
	void clearScreen();
}

#endif