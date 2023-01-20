#pragma once

#include "scene.hpp"

enum GuiWindowsid
{
	GUI_WINDOW_ID_LEVEL_EDITOR,
	GUI_WINDOW_ID_WAVE_EDITOR,
	GUI_WINDOW_ID_CHAT_CONSOLE,

	GUI_WINDOW_ID_COUNT
};

class Gui
{
private:
	static void HandleMenuBar();
	static void HandleNetworkMenuBar();

	static char mServerHostName[100];

public:
	static bool openedWindows[GUI_WINDOW_ID_COUNT];

	static void CreateGuiWindow(GuiWindowsid id);
	static void Update();

	static Scene UpdateMainMenu();
	static Scene UpdateLevelSelection();
	static Scene UpdateFreeplaySelection();
	static Scene UpdateOptions();
};
