#ifndef LIGHTNING_FS
#define LIGHTNING_FS

#include <string>
#include <vector>

namespace Lightning
{
	namespace FS
	{
		inline struct Dir
		{
			struct File
			{
				Dir* parentDir{};				
				std::string name{};
				std::string content{};
			};

			Dir* parentDir{};
			std::string name{};
			std::vector<File> files{};
			std::vector<Dir> dirs{};
		} fs{};

		void loadFilesystem();
		void saveFilesystem();
	}
}

#endif