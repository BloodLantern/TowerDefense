#include "Globals.h"

ImDrawList* Globals::gDrawList;
ImGuiIO* Globals::gIO;
int32_t Globals::gWindowX;
int32_t Globals::gWindowY;

void Globals::UpdateGlobals()
{
	Globals::gDrawList = ImGui::GetBackgroundDrawList();
	Globals::gIO = &ImGui::GetIO();
}
