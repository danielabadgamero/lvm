#ifndef DEV_H
#define DEV_H

#include <functional>

namespace Dev
{
	inline std::function<void(int)> devices[256]{};
	
	enum Device
	{
		CONSOLE,
		GRAPHICS,
	};

	void init();
}

#endif
