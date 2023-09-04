#ifndef DEV_H
#define DEV_H

#include <functional>

namespace Dev
{
	inline std::function<void(unsigned shot, unsigned short)> devices[4][4]{};
	
	void init();
}

#endif
