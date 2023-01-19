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

	if (ImGui::BeginMenu("Menus"))
	{
		if (ImGui::MenuItem("Main menu"))
			Globals::gGame->SetScene(Scene::MAIN_MENU);

		ImGui::Separator();

		if (ImGui::MenuItem("Close game"))
			Globals::gClosingGame = true;

		ImGui::EndMenu();
	}


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

	ImGui::Separator();

	if (ImGui::MenuItem("Open chat"))
		Gui::CreateGuiWindow(GUI_WINDOW_ID_CHAT_CONSOLE);

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
				break;

			case GUI_WINDOW_ID_CHAT_CONSOLE:
				ChatConsole::Draw();
		}
	}
}

Scene Gui::UpdateMainMenu()
{
	Scene result = Scene::MAIN_MENU;

	ImGui::SetNextWindowPos(ImVec2(Globals::gGridX, Globals::gGridY));
	ImGui::SetNextWindowSize(ImVec2(Globals::gWindowWidth, Globals::gWindowHeight - GRID_OFFSET_Y));

	if (ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushFont(Globals::gFontBig);
		ImGui::Dummy(ImVec2(1, 50));
		ImGui::SetCursorPosX(Globals::gWindowWidth / 2.f - ImGui::CalcTextSize("Fourmi defense").x / 2);
		ImGui::Text("Fourmi defense");
		ImGui::PopFont();

		ImGui::PushFont(Globals::gFontSemiBig);

		const ImVec2 buttonSize(130, 50);
		ImVec2 cursor(Globals::gWindowWidth / 2.f - buttonSize.x / 2.f, ImGui::GetCursorPosY() + 150);

		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Play", buttonSize))
			result = Scene::LEVEL_SELECTION;

		cursor.y += 100;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Freeplay", buttonSize))
			result = Scene::FREEPLAY_SELECTION;

		cursor.y += 100;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Bestiary", buttonSize))
			result = Scene::BESTIARY;

		cursor.y += 100;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Options", buttonSize))
			result = Scene::OPTIONS;

		ImGui::PopFont();
	}

	ImGui::End();
	return result;
}

Scene Gui::UpdateLevelSelection()
{
	Scene result = Scene::LEVEL_SELECTION;

	uint8_t amountOfLevels = Globals::gGame->CountLevels();

	ImGui::SetNextWindowPos(ImVec2(Globals::gGridX, Globals::gGridY));
	ImGui::SetNextWindowSize(ImVec2(Globals::gWindowWidth, Globals::gWindowHeight - GRID_OFFSET_Y));

	if (ImGui::Begin("##levelsel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushFont(Globals::gFontBig);
		ImGui::Dummy(ImVec2(1, 50));
		ImGui::SetCursorPosX(Globals::gWindowWidth / 2.f - ImGui::CalcTextSize("Levels").x / 2);
		ImGui::Text("Levels");
		ImGui::PopFont();

		ImGui::PushFont(Globals::gFontSemiBig);

		const ImVec2 buttonSize(320, 160);
		ImVec2 cursor(Globals::gWindowWidth / 4.f - buttonSize.x / 2.f, ImGui::GetCursorPosY() + 150);

		for (int32_t i = 1; i < amountOfLevels + 1; i++)
		{
			std::string id = std::to_string(i);
			std::string previewFile = std::string("previews\\LevelPreview").append(id);
			Texture* tex = Globals::gResources->GetTexture(previewFile);

			ImGui::SetCursorPos(cursor);

			cursor.x += 350;

			if (ImGui::ImageButton(tex->id, buttonSize))
			{
				Globals::gGame->StartLevel(i);
				result = Scene::IN_GAME;
				break;
			}

			if ((i % 3) == 0)
			{
				cursor.x -= 350 * 3;
				cursor.y += 220;
			}
		}

		if (ImGui::Button("Back", ImVec2(130, 50)))
			result = Scene::MAIN_MENU;

		ImGui::PopFont();
	}

	ImGui::End();
	return result;
}

Scene Gui::UpdateFreeplaySelection()
{
	Scene result = Scene::FREEPLAY_SELECTION;

	ImGui::SetNextWindowPos(ImVec2(Globals::gGridX, Globals::gGridY));
	ImGui::SetNextWindowSize(ImVec2(Globals::gWindowWidth, Globals::gWindowHeight - GRID_OFFSET_Y));

	if (ImGui::Begin("##freeplay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushFont(Globals::gFontBig);
		ImGui::Dummy(ImVec2(1, 50));
		ImGui::SetCursorPosX(Globals::gWindowWidth / 2.f - ImGui::CalcTextSize("Freeplay").x / 2);
		ImGui::Text("Freeplay");
		ImGui::PopFont();

		ImGui::PushFont(Globals::gFontSemiBig);

		const ImVec2 buttonSize(130, 50);
		ImVec2 cursor(Globals::gWindowWidth / 2.f - buttonSize.x / 2.f, ImGui::GetCursorPosY() + 150);

		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Back", buttonSize))
			result = Scene::MAIN_MENU;

		// TODO freeplay info, records, stats...

		ImGui::PopFont();
	}

	ImGui::End();
	return result;
}

Scene Gui::UpdateOptions()
{
	Scene result = Scene::OPTIONS;

	ImGui::SetNextWindowPos(ImVec2(Globals::gGridX, Globals::gGridY));
	ImGui::SetNextWindowSize(ImVec2(Globals::gWindowWidth, Globals::gWindowHeight - GRID_OFFSET_Y));

	if (ImGui::Begin("##options", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushFont(Globals::gFontBig);
		ImGui::Dummy(ImVec2(1, 50));
		ImGui::SetCursorPosX(Globals::gWindowWidth / 2.f - ImGui::CalcTextSize("Options").x / 2);
		ImGui::Text("Options");
		ImGui::PopFont();

		ImGui::PushFont(Globals::gFontSemiBig);

		const ImVec2 buttonSize(130, 50);
		ImVec2 cursor(Globals::gWindowWidth / 2.f - buttonSize.x / 2.f, ImGui::GetCursorPosY() + 150);

		ImGui::SetCursorPos(cursor);
		ImGui::Checkbox("Fullscreen", &Globals::gFullscreen);

		cursor.y += 150;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Back", buttonSize))
			result = Scene::MAIN_MENU;

		// TODO network stuff probably

		ImGui::PopFont();
	}

	ImGui::End();
	return result;
}


