#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <filesystem>

#include "Core.h"
#include "Commands.h"
#include "Errors.h"

void Core::init()
{
	Commands::init();

	running = true;
}

void Core::loop()
{
	std::filesystem::path currentPath{ std::filesystem::current_path() };
	std::cout << currentPath.string() << "> ";

	std::string input{};
	std::getline(std::cin, input);
	
	std::vector<std::string> words(1);
	bool string{};
	for (const char& c : input)
		if (c == ' ' && !string) words.push_back("");
		else if (c == '"') string ^= 1;
		else words.back().push_back(c);

	std::unordered_map<std::string, std::string> args{};
	for (std::string& word : words)
		if (word.find('=') == std::string::npos)
			args["default"] = word;
		else
		{
			std::string name{ word.substr(0, word.find('=')) };
			std::string value{ word.substr(word.find('=') + 1) };
			args[name] = value;
		}

	if (words[0].empty()) E1;
	if (!Commands::functions.contains(words[0])) E2;

	Commands::functions[words.at(0)](args);
	std::cout << '\n';
}

void Core::quit()
{

}