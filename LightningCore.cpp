<<<<<<< HEAD
#include <iostream>
#include <fstream>
#include <conio.h>

#include "LightningCore.h"
#include "LightningFS.h"

#undef OUT

void Lightning::init()
{
	do
	{
		CPU.REG[IR] = ROM[CPU.PC];
		CPU.REG[IR] <<= 8;
		CPU.REG[IR] += (unsigned char)ROM[CPU.PC + 1];
		CPU.REG[IR] <<= 8;
		CPU.REG[IR] += (unsigned char)ROM[CPU.PC + 2];
		CPU.REG[IR] <<= 8;
		CPU.REG[IR] += (unsigned char)ROM[CPU.PC + 3];
 		CPU.process();
		CPU.PC += 4;
	} while (CPU.REG[IR]);

	CPU.PC = 0;
	running = true;
}

void Lightning::CPU::process()
{
	switch (REG[IR] >> 24)
	{
	case HALT:
		running = false;
		break;
	case SET:
		REG[(REG[IR] & Rd) >> 16] = REG[IR] & imm16;
		break;
	case SAC:
		REG[AC] = REG[IR] & imm24;
		break;
	case SAR:
		REG[AR] = REG[IR] & imm24;
		break;
	case SDR:
		REG[DR] = REG[IR] & imm24;
		break;
	case COUTI:
		std::cout << static_cast<char>((REG[IR] & imm24) >> 16) << static_cast<char>((REG[IR] & imm24) >> 8) << static_cast<char>(REG[IR] & imm24);
		break;
	case COUT:
		std::cout << static_cast<char>(REG[(REG[IR] & Rs1) >> 8]);
		break;
	case RMEM:
		REG[DR] = RAM[REG[AR]];
		break;
	case WMEM:
		RAM[REG[AR]] = static_cast<unsigned char>(REG[DR]);
		break;
	case INC:
		REG[(REG[IR] & Rd) >> 16] += static_cast<char>(REG[IR] & imm16);
		break;
	case CIN:
		REG[(REG[IR] & Rd) >> 16] = _getch();
		break;
	case RFS:
		REG[DR] = FS::filesystem[REG[AR]];
		break;
	case WFS:
		FS::filesystem[REG[AR]] = static_cast<char>(REG[DR]);
		break;
	case JMP:
		Lightning::CPU.PC = REG[(REG[IR] & Rs1) >> 8] * 4;
		break;
	case JPI:
		Lightning::CPU.PC += static_cast<char>((REG[IR] & imm24) * 4 - 4);
		break;
	case JPZ:
		Lightning::CPU.PC = ((REG[REG[IR] & Rs2] == 0) ? (REG[(REG[IR] & Rs1) >> 8] * 4) - 4 : Lightning::CPU.PC);
		break;
	case JPZI:
		Lightning::CPU.PC += ((REG[(REG[IR] & Rs1) >> 8] == 0) ? static_cast<char>((REG[IR] & imm8) * 4 - 4) : 0);
		break;
	case JNZ:
		Lightning::CPU.PC = ((REG[REG[IR] & Rs2] != 0) ? (REG[(REG[IR] & Rs1) >> 8] * 4) - 4 : Lightning::CPU.PC);
		break;
	case JNZI:
		Lightning::CPU.PC += ((REG[(REG[IR] & Rs1) >> 8] != 0) ? static_cast<char>((REG[IR] & imm8) * 4 - 4) : 0);
		break;
	case PUSH:
		stack.push(REG[(REG[IR] & Rs1) >> 8]);
		break;
	case POP:
		REG[(REG[IR] & Rd) >> 16] = stack.top();
		stack.pop();
		break;
	case CALL:
		stack.push(Lightning::CPU.PC);
		Lightning::CPU.PC = REG[(REG[IR] & Rs1) >> 8] * 4 - 4;
		break;
	case RET:
		Lightning::CPU.PC = stack.top();
		stack.pop();
		break;
	default:
		REG[(REG[IR] & Rd) >> 16] = ALU.process(static_cast<int>(REG[IR] >> 24), REG[(REG[IR] & Rs1) >> 8], REG[(REG[IR] & Rs2)]);
	}
}

long long Lightning::CPU::ALU::process(int opcode, long long rs1, long long rs2)
{
	switch (opcode)
	{
	case ADD:	return rs1 + rs2;
	case SUB:	return rs1 - rs2;
	case MUL:	return rs1 * rs2;
	case DIV:	return rs1 / rs2;
	case MOD:	return rs1 % rs2;
	case AND:	return rs1 & rs2;
	case OR:	return rs1 | rs2;
	case XOR:	return rs1 ^ rs2;
	case NOT:	return ~rs1;
	case CPY:	return rs1;
	case SEQ:	return rs1 == rs2;
	case SGT:	return rs1 > rs2;
	case SLT:	return rs1 < rs2;
	case LSFT:	return rs1 << rs2;
	case RSFT:	return rs1 >> rs2;

	default:	return -1;
	}
=======
#include "LightningCore.h"
#include "LightningGUI.h"
#include "LightningFS.h"

#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

void Lightning::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Lightning VM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() }; (void)io;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	FS::loadFilesystem();

	running = true;
}

void Lightning::processEvents()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(window, &screen.w, &screen.h);
			break;
		}
	}
}

void Lightning::draw()
{
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui::NewFrame();

	GUI::drawGui();

	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x40, 0xff);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	GUI::draw();

	SDL_RenderPresent(renderer);
}

void Lightning::quit()
{
	FS::saveFilesystem();

	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	window = nullptr;
	renderer = nullptr;

	SDL_Quit();
>>>>>>> gui_2
}