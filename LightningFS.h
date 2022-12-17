#ifndef LIGHTNING_FS
#define LIGHTNING_FS

#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

namespace Lightning
{
	namespace FS
	{
		inline struct Dir
		{
			struct File
			{
				std::string name{};
				std::string content{};
				std::vector<std::string> contentVector{};
			};

			std::string name{};
			std::vector<File> files{};
			std::vector<Dir*> subDirs{};
		} Filesystem{};

		inline std::vector<Dir*> path{ &Filesystem };
		inline Dir::File* targetFile{ nullptr };

		inline std::ifstream fs_in{};
		inline std::ofstream fs_out{};

		void loadFilesystem();
		void saveFilesystem();
		void writeFilesystem(Dir*);
		std::string getPath();
		void printPath();
		void printFileContent();
	}
}

#endif