#include <map>
#include <string>
#include <vector>
#include <stdexcept>

#include "LightningCore.h"
#include "LightningCMD.h"
#include "LightningTEXT.h"

void Lightning::TEXT::addLine(std::map<std::string, std::string>* arguments, std::vector<std::string>* content)
{
	if (arguments->contains("content"))
		if (arguments->contains("line"))
		{
			int line{};
			try
			{
				line = std::stoi(arguments->at("line"));
			}
			catch (std::invalid_argument e)
			{
				std::cout << e.what() << '\n';
				return;
			}
			if (line >= 1 && line <= content->size())
				content->insert(content->begin() + std::stoi(arguments->at("line")) - 1, arguments->at("content"));
		}
		else
			content->push_back(arguments->at("content"));
}

void Lightning::TEXT::remLine(std::map<std::string, std::string>* arguments, std::vector<std::string>* content)
{
	if (arguments->contains("line"))
	{
		int line{};
		try
		{
			line = std::stoi(arguments->at("line")) - 1;
		}
		catch (std::invalid_argument e)
		{
			std::cout << e.what() << '\n';
			return;
		}
		if (line >= 0 && line < content->size())
			content->erase(content->begin() + line);
	}
	else if (content->size() > 0)
		content->pop_back();
}

void Lightning::TEXT::close()
{
	mode = Mode::CMD;
	targetFile = nullptr;
}