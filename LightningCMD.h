#ifndef LIGHTNING_CMD
#define LIGHTNING_CMD

#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "LightningCore.h"

namespace Lightning
{
	namespace CMD
	{
		void exit();
		void help(std::map<std::string, std::string>* arguments);
		void mkdir(std::map<std::string, std::string>* arguments);
		void rmdir(std::map<std::string, std::string>* arguments);
		void cd(std::map<std::string, std::string>* arguments);
		void ls();
		void touch(std::map<std::string, std::string>* arguments);
		void rm(std::map<std::string, std::string>* arguments);
		void print(std::map<std::string, std::string>* arguments);
		void open(std::map<std::string, std::string>* arguments);
	}
}

#endif