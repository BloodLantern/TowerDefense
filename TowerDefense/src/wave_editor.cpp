#include "wave_editor.hpp"
#include "gui.hpp"
#include "imgui.h"

void WaveEditor::Update()
{
	if (ImGui::Begin("Wave editor", &Gui::openedWindows[GUI_WINDOW_ID_WAVE_EDITOR]))
	{

	}

	ImGui::End();
}
