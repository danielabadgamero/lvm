#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <string>
#include <vector>

std::vector<std::string> keywords
{
	"wb",	// write byte
	"wd",	// write double byte
	"wq",	// write quad byte
	"ws",	// write characters
};

enum Keyword
{
	wb,
	wd,
	wq,
	ws,
};

#endif