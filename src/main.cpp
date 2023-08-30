#include "Core.h"

int main()
{
	Core::init();
	Core::loop();
	Core::quit();

	return 0;
}
