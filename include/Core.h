#ifndef CORE_H
#define CORE_H

#include <string>
#include <functional>
#include <unordered_map>

namespace Core
{
	inline bool running{ true };
	inline bool executing{};

	inline std::unordered_map<std::string, std::string> desc{};
	inline std::unordered_map<std::string, std::function<void(const std::string&)>> commands{};
	inline std::unordered_map<std::string, std::function<void()>> registered{};

	void init();
	void loop();
	void quit();
}

#endif
