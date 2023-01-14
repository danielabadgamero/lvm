#include "LightningCore.h"
#include "LightningCHAR.h"

void Lightning::CHAR::loadFont()
{
	SDL_memcpy(RAM + FONT_CH + 32 * ('a' - ' '), a, 32 * 4);
}