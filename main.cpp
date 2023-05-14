#include "Core.h"
#include "Text.h"
#include "Lightning.h"

int main(int, char**)
{
	Core::init();

	while (Core::running)
		switch (Core::mode)
		{
		case Core::CMD:
			Core::loop();
			break;
		case Core::TXT:
			Text::loop();
			break;
		case Core::EXE:
			Lightning::loop();
			break;
		}

	Core::quit();

	return 0;
}