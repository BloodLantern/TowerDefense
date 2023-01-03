#pragma once

#include <imgui.h>
#include <stdint.h>

class Globals
{
public:
	static ImDrawList* gDrawList;
	static ImGuiIO* gIO;

	static int32_t gWindowX;
	static int32_t gWindowY;

	static void UpdateGlobals();

	
};
