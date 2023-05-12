#include "Core.h"

int main()
{
	Core::init();

	while (Core::running)
		Core::loop();

	Core::quit();

	return 0;
}