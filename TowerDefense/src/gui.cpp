#include "gui.hpp"
#include "level_editor.hpp"
#include "wave_editor.hpp"

#include <imgui.h>

#include <stdint.h>

bool Gui::openedWindows[GUI_WINDOW_ID_COUNT];


void Gui::HandleMenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	if (ImGui::BeginMenu("Editors"))
	{
		if (ImGui::MenuItem("Level editor"))
			Gui::CreateWindow(GUI_WINDOW_ID_LEVEL_EDITOR);

		if (ImGui::MenuItem("Wave editor"))
			Gui::CreateWindow(GUI_WINDOW_ID_WAVE_EDITOR);

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void Gui::CreateWindow(GuiWindowsid id)
{
	if (Gui::openedWindows[id])
	{
		// TODO set focus
		return;
	}

	Gui::openedWindows[id] = true;
}

void Gui::Update()
{
	Gui::HandleMenuBar();

	for (uint32_t i = 0; i < GUI_WINDOW_ID_COUNT; i++)
	{
		if (!Gui::openedWindows[i])
			continue;

		switch (i)
		{
			case GUI_WINDOW_ID_LEVEL_EDITOR:
				LevelEditor::Update();
				break;

			case GUI_WINDOW_ID_WAVE_EDITOR:
				WaveEditor::Update();
		}
	}
}

