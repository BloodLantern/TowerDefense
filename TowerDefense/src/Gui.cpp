#include "Gui.h"
#include "LevelEditor.h"

#include <imgui.h>

#include <stdint.h>

bool Gui::m_openedWindows[GUI_WINDOW_ID_COUNT];


void Gui::handleMenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	if (ImGui::BeginMenu("Editors"))
	{
		if (ImGui::MenuItem("Level editor"))
			Gui::createWindow(GUI_WINDOW_ID_LEVEL_EDITOR);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void Gui::createWindow(GuiWindowsid id)
{
	if (Gui::m_openedWindows[id])
	{
		// TODO set focus
		return;
	}

	Gui::m_openedWindows[id] = true;
}

void Gui::update()
{
	Gui::handleMenuBar();

	for (uint32_t i = 0; i < GUI_WINDOW_ID_COUNT; i++)
	{
		if (!Gui::m_openedWindows[i])
			continue;

		switch (i)
		{
			case GUI_WINDOW_ID_LEVEL_EDITOR:
				LevelEditor::update();
		}
	}
}

