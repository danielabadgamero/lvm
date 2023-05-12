#ifndef CORE_H
#define CORE_H

namespace Core
{
	inline enum
	{
		CMD,
		TXT,
		EXE
	} mode{};

	inline bool running{};

	void init();
	void loop();
	void quit();
}

#endif