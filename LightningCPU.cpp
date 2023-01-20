#include "LightningCore.h"
#include "LightningCPU.h"

namespace Format
{
	constexpr static char opcode{ 0b11110000 };
}

static void decode()
{
	using namespace Lightning;
	switch (RAM[CPU::reg[CPU::pc]] & Format::opcode)
	{
	case 0x0:
		break;
	case 0x1:
		break;
	case 0x2:
		break;
	case 0x3:
		break;
	case 0x4:
		break;
	case 0x5:
		break;
	case 0x6:
		break;
	case 0x7:
		break;
	case 0x8:
		break;
	case 0x9:
		break;
	case 0xA:
		break;
	case 0xB:
		break;
	case 0xC:
		break;
	case 0xD:
		break;
	case 0xE:
		break;
	case 0xF:
		break;
	}
}

int Lightning::CPU::cycle(void*)
{
	while (~bistables.running)
	{
		
	}

	while (~bistables.running)
	{
	}

	return 0;
}