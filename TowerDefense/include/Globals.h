#pragma once

#include <Vector2.h>
#include <imgui.h>
#include <stdint.h>

class Globals
{
public:
	static ImDrawList* gDrawList;
	static ImGuiIO* gIO;

	static int32_t gWindowX;
	static int32_t gWindowY;
	static int32_t gWindowWidth;
	static int32_t gWindowHeight;

	static bool fullscreen;

	static void updateGlobals();
};
