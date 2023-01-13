#include "round_editor.hpp"
#include "gui.hpp"
#include "imgui.h"

#include <iostream>

static const char* const sCommandNames[] = {
    "Enemy",
    "Cooldown",
    "GFX effect",
    "SFX",
    "End"
};

static const char* const sEnemyNames[] = {
    "Base"
};

std::vector<RoundInfo> RoundEditor::mRoundInfo;

void RoundEditor::Update()
{
    //ImGui::ShowDemoWindow();
    //return;
	if (ImGui::Begin("Wave editor", &Gui::openedWindows[GUI_WINDOW_ID_WAVE_EDITOR]))
	{
        if (ImGui::Button("Add"))
            mRoundInfo.push_back(RoundInfo(ROUND_COMMAND_SPAWN_ENEMY, 0u));

        ImGui::SameLine();
        RoundEditor::HandleClear();

        DisplayTable();
	}

	ImGui::End();
}

void RoundEditor::DisplayTable()
{
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + 80, cursor.y));
    if (!ImGui::BeginTable("Command table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        return;

    ImGui::TableSetupColumn("Command");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();

    for (size_t row = 0; row < mRoundInfo.size(); row++)
    {
        RoundInfo* round = &mRoundInfo[row];

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::PushID(round);

        ImGui::SetNextItemWidth(-FLT_MIN);

        RoundCommand prev = round->command;
        ImGui::Combo("##combo", (int*)&round->command, sCommandNames, IM_ARRAYSIZE(sCommandNames));
        if (prev != round->command)
            round->data.dataInt = 0;
        
        ImGui::TableNextColumn();
        DisplayParamColumn(round);
        ImGui::PopID();
    }
    ImGui::EndTable();

    ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 20));
    for (size_t row = 0; row < mRoundInfo.size(); row++)
    {
        ImGui::PushID(row);
        if (ImGui::Button("Delete"))
        {
            mRoundInfo.erase(mRoundInfo.begin() + row);
        }
        ImGui::PopID();
    }
}

void RoundEditor::DisplayParamColumn(RoundInfo* round)
{
    ImGui::SetNextItemWidth(-FLT_MIN);
    switch (round->command)
    {
        case ROUND_COMMAND_SPAWN_ENEMY:
            ImGui::Combo("##input", (int*)&round->data.dataInt, sEnemyNames, IM_ARRAYSIZE(sEnemyNames));
            break;

        case ROUND_COMMAND_COOLDOWN:
            ImGui::InputFloat("##input", &round->data.dataFloat, 0.1f, 1.f);
            break;

        case ROUND_COMMAND_GFX_EFFECT:
            ImGui::InputScalar("##input", ImGuiDataType_U32, &round->data.dataInt);
            break;

        case ROUND_COMMAND_PLAY_SOUND:
            ImGui::InputScalar("##input", ImGuiDataType_U32, &round->data.dataInt);
            break;

        case ROUND_COMMAND_END:
            uint32_t step = 10;
            uint32_t stepFast = 100;
            ImGui::InputScalar("##input", ImGuiDataType_U32, &round->data.dataInt, &step, &stepFast, "%d");
    }
}

void RoundEditor::HandleClear()
{
    if (ImGui::Button("Clear"))
        ImGui::OpenPopup("Delete?");

    if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to clear everything?");

        if (ImGui::Button("Yes", ImVec2(120, 0)))
        {
            mRoundInfo.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("No", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}
