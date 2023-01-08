#include "Globals.h"
#include "PlayField.h"

ImDrawList* Globals::gDrawList;
ImGuiIO* Globals::gIO;

PlayField* Globals::gPlayField;

int32_t Globals::gWindowX;
int32_t Globals::gWindowY;
int32_t Globals::gWindowWidth;
int32_t Globals::gWindowHeight;

int32_t Globals::gGridX;
int32_t Globals::gGridY;

bool Globals::fullscreen = false;

void Globals::updateGlobals()
{
	gDrawList = ImGui::GetBackgroundDrawList();
	gIO = &ImGui::GetIO();
	gGridX = gWindowX + GRID_OFFSET_X;
	gGridY = gWindowY + GRID_OFFSET_Y;
}

void Globals::bindPlayField(PlayField* pf)
{
	gPlayField = pf;
}
