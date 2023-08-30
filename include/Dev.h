#ifndef DEV_H
#define DEV_H

#include <functional>

namespace Dev
{
	inline std::function<void(int)> devices[128]{};
	
	enum Output_Dev
	{
		CONSOLE,
	};

	void init();
}

#endif
