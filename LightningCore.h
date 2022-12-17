#ifndef LIGHTNING_CORE
#define LIGHTNING_CORE

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>

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
	inline Dir FileSystem{};
	inline std::vector<Dir*> path{ &FileSystem };
	inline Dir::File* targetFile{ nullptr };
	inline MODE mode{ MODE::CMD };

	inline int* addr{ RAM };
	inline bool running{ true };

	void loadFilesystem();

	std::string getPath();

	void writeFilesystem(Dir* dir);

	void saveFilesystem();

	std::vector<std::string> fileToVector();

	void vectorToFile(std::vector<std::string>* content);

	void printPath();

	void printFileContent();

	void handleCommand(std::string* command, std::map<std::string, std::string>* arguments);
}

#endif