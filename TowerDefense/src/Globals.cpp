#include "globals.hpp"
#include "playfield.hpp"

ImDrawList* Globals::gDrawList;
ImGuiIO* Globals::gIO;

Game* Globals::gGame;
DiscordRichPresence Globals::gDiscordRpc;

int32_t Globals::gWindowX;
int32_t Globals::gWindowY;
int32_t Globals::gWindowWidth;
int32_t Globals::gWindowHeight;

int32_t Globals::gGridX;
int32_t Globals::gGridY;

bool Globals::fullscreen = false;

void Globals::UpdateGlobals()
{
	gDrawList = ImGui::GetBackgroundDrawList();
	gIO = &ImGui::GetIO();
	gGridX = gWindowX + GRID_OFFSET_X;
	gGridY = gWindowY + GRID_OFFSET_Y;
}

void Globals::BindGame(Game* g)
{
	gGame = g;
}
