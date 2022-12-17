#ifndef LIGHTNING_TEXT
#define LIGHTNING_TEXT

#include <map>
#include <string>

#include "LightningCore.h"
#include "LightningCMD.h"

namespace Lightning
{
	namespace TEXT
	{
		void addLine(std::map<std::string, std::string>* arguments, std::vector<std::string>* content);
		void remLine(std::map<std::string, std::string>* arguments, std::vector<std::string>* content);
		void close();
	}
}

#endif