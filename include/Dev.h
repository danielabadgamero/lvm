#ifndef DEV_H
#define DEV_H

#include <functional>

namespace Dev
{
	inline std::function<void(unsigned shot)> devices[0xff][0xff]{};
	
	void init();
}

#endif
