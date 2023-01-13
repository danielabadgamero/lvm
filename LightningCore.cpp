#include <SDL.h>

#include "LightningCore.h"
#include "LightningVGA.h"

void Lightning::init()
{
	running = true;

	SDL_Thread* VGAThread{ SDL_CreateThread(VGA::LightningMain, "VGA", nullptr) };
}

void Lightning::cycle()
{

}

void Lightning::quit()
{

}