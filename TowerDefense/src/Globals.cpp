#include "Globals.h"

ImDrawList* Globals::gDrawList;
ImGuiIO* Globals::gIO;
int32_t Globals::gWindowX;
int32_t Globals::gWindowY;
int32_t Globals::gWindowWidth;
int32_t Globals::gWindowHeight;
bool Globals::fullscreen = false;

void Globals::updateGlobals()
{
	gDrawList = ImGui::GetBackgroundDrawList();
	gIO = &ImGui::GetIO();
}
