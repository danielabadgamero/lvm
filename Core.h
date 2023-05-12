#ifndef CORE_H
#define CORE_H

namespace Core
{
	inline bool running{};

	void init();
	void loop();
	void quit();
}

#endif