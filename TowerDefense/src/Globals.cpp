#include "Globals.h"

ImDrawList* Globals::gDrawList;
ImGuiIO* Globals::gIO;
int32_t Globals::gWindowX;
int32_t Globals::gWindowY;
bool Globals::fullscreen = false;

int32_t Globals::gWindowWidth = 1350;
int32_t Globals::gWindowHeight = 720;

void Globals::updateGlobals()
{
	Globals::gDrawList = ImGui::GetBackgroundDrawList();
	Globals::gIO = &ImGui::GetIO();
}
