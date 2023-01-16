#pragma once

enum GuiWindowsid
{
	GUI_WINDOW_ID_LEVEL_EDITOR,
	GUI_WINDOW_ID_WAVE_EDITOR,

	GUI_WINDOW_ID_COUNT
};

class Gui
{
private:

	static void HandleMenuBar();

public:
	static bool openedWindows[GUI_WINDOW_ID_COUNT];

	static void CreateWindow(GuiWindowsid id);
	static void Update();
};
