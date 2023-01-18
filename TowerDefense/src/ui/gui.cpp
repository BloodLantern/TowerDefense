#include "gui.hpp"
#include "level_editor.hpp"
#include "round_editor.hpp"
#include "network/chat_console.hpp"
#include "network/network_interface.hpp"
#include "globals.hpp"

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
			Gui::CreateGuiWindow(GUI_WINDOW_ID_LEVEL_EDITOR);

		if (ImGui::MenuItem("Wave editor"))
			Gui::CreateGuiWindow(GUI_WINDOW_ID_WAVE_EDITOR);

		ImGui::EndMenu();
	}

	Gui::HandleNetworkMenuBar();

	ImGui::EndMainMenuBar();
}

void Gui::HandleNetworkMenuBar()
{
	if (!ImGui::BeginMenu("Network"))
		return;
	
	if (Globals::gNetwork.IsServerStarted())
	{
		if (ImGui::MenuItem("Stop server"))
			Globals::gNetwork.StopServer();
	}
	else
	{
		if (ImGui::MenuItem("Start server"))
			Globals::gNetwork.StartServer();
	}

	ImGui::Separator();

	if (Globals::gNetwork.client->IsConnected())
	{
		if (ImGui::MenuItem("Disconnect"))
			Globals::gNetwork.StopClient();
	}
	else
	{
		if (ImGui::MenuItem("Connect to server"))
			Globals::gNetwork.StartClient("ZOTAC-12");
	}

	ImGui::EndMenu();
}

void Gui::CreateGuiWindow(GuiWindowsid id)
{
	if (Gui::openedWindows[id])
	{
		// TODO: set focus
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
				RoundEditor::Update();
		}
	}

	ChatConsole::Draw();
}

