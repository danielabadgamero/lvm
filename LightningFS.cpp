#include "LightningFS.h"

#include <fstream>
#include <string>
#include <vector>

static std::string getPath(Lightning::FS::Dir dir)
{
	if (dir.name.empty())
		return "";
	else
		return getPath(*dir.parentDir) + "/" + dir.name;
}
/*
static std::string getPath(Lightning::FS::Dir::File file)
{
	if (file.parentDir->name.empty())
		return "";
	else
		return getPath(*file.parentDir) + "/" + file.name;
}
*/
static std::string loopDirs(Lightning::FS::Dir parentDir)
{
	std::string filesystem{};
	for (Lightning::FS::Dir dir : parentDir.dirs)
		if (dir.dirs.size() != 0)
			filesystem.append(loopDirs(dir) + "\n");
		else
			filesystem.append(getPath(dir));
	return filesystem;
}

void Lightning::FS::loadFilesystem()
{
	std::ifstream in{ "fs", std::ifstream::binary };


}

void Lightning::FS::saveFilesystem()
{
	std::ofstream out{ "fs", std::ofstream::binary };

	std::string dirs{ loopDirs(fs) };
	out.write(dirs.c_str(), dirs.size());
}