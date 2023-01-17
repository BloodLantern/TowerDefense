#include "globals.hpp"
#include "playfield.hpp"

ImDrawList* Globals::gDrawList;
ImGuiIO* Globals::gIO;

ImFont* Globals::gFontBig;
ImFont* Globals::gFontSemiBig;
ImFont* Globals::gFontMedium;

Game* Globals::gGame;
Resources* Globals::gResources;
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

void Globals::InitGlobals(Game* g)
{
	gGame = g;
	gResources = new Resources;

	Tower::InitUITextures();
}

void Globals::DestroyGlobals()
{
	delete gResources;
}
