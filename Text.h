#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <fstream>
#include <string>

namespace Text
{
	inline std::vector<std::string> file{};
	inline std::string name{};

	void init(std::ifstream&, std::string&);
	void loop();
}

#endif