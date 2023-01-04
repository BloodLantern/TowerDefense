#pragma once

enum GuiWindowsid
{
	GUI_WINDOW_ID_LEVEL_EDITOR,

	GUI_WINDOW_ID_COUNT
};

class Gui
{
private:

	static void handleMenuBar();

public:
	static bool m_openedWindows[GUI_WINDOW_ID_COUNT];

	static void createWindow(enum GuiWindowsid id);
	static void update();
};
