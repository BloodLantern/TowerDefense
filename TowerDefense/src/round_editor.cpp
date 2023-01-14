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
    "Ant",
    "Mite",
    "Gold scarab",
    "Ladybug",
};

std::vector<RoundInfo> RoundEditor::mRoundInfo;
char RoundEditor::mFileName[30];


void RoundEditor::Update()
{
	if (ImGui::Begin("Wave editor", &Gui::openedWindows[GUI_WINDOW_ID_WAVE_EDITOR]))
	{
        if (ImGui::Button("Add"))
            mRoundInfo.push_back(RoundInfo(ROUND_COMMAND_SPAWN_ENEMY, 0u));

        ImGui::SameLine();
        RoundEditor::HandleClear();
        ImGui::SameLine();
        RoundEditor::HandleSaveLoad();

        RoundEditor::DisplayTable();
	}

	ImGui::End();
}

void RoundEditor::DisplayTable()
{
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + 130, cursor.y));
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

        int32_t _cmd = round->command;
        ImGui::Combo("##combo", &_cmd, sCommandNames, IM_ARRAYSIZE(sCommandNames));
        round->command = static_cast<RoundCommand>(_cmd);
        if (prev != round->command)
            round->data.dataInt = 0;
        
        ImGui::TableNextColumn();
        DisplayParamColumn(round);
        ImGui::PopID();
    }
    ImGui::EndTable();

    ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 20));
    for (std::vector<RoundInfo>::iterator _r = mRoundInfo.begin(); _r != mRoundInfo.end(); )
    {
        RoundInfo* round = &*_r;
        ImGui::PushID(round);

        if (ImGui::Button("Delete"))
        {
            _r = mRoundInfo.erase(_r);
            ImGui::PopID();
            continue;
        }
        ImGui::SameLine();
        if (ImGui::Button("Insert"))
        {
            _r = mRoundInfo.insert(_r, RoundInfo(ROUND_COMMAND_SPAWN_ENEMY, 0u));
            ImGui::PopID();
            continue;
        }

        ImGui::PopID();
        _r++;
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

void RoundEditor::HandleSaveLoad()
{
    ImGui::Dummy(ImVec2(80, 0));
    ImGui::SameLine();

    if (ImGui::Button("Save"))
        Save(mRoundInfo, std::string(WAVES_PATH).append(mFileName).c_str());

    ImGui::SameLine();

    if (ImGui::Button("Load"))
        Load(mRoundInfo, std::string(WAVES_PATH).append(mFileName).c_str());

    ImGui::SameLine();
    ImGui::Text("File name");
    ImGui::SameLine();
    ImGui::InputText("FileNameInput", RoundEditor::mFileName, sizeof(RoundEditor::mFileName));
}

// Temp?

bool RoundEditor::Load(std::vector<RoundInfo>& dst, const char* const src)
{
    FILE* f;
    fopen_s(&f, src, "rb");

    if (!f)
    {
        std::cerr << "ERROR - Loading round info : Couldn't open " << src << " file" << std::endl;
        return false;
    }

    dst.clear();

    while (true)
    {
        int32_t command = fgetc(f);

        if (command == EOF)
            break;

        uint32_t data;
        fread(&data, sizeof(data), 1, f);
        dst.push_back(RoundInfo(static_cast<RoundCommand>(command), data));
    }

    fclose(f);
    return true;
}

void RoundEditor::Save(std::vector<RoundInfo>& src, const char* const dst)
{
    FILE* f;
    fopen_s(&f, dst, "wb");

    if (!f)
    {
        std::cerr << "ERROR - Saving round info : Couldn't open " << dst << " file" << std::endl;
        return;
    }

    for (size_t i = 0; i < src.size(); i++)
    {
        fwrite(&src[i].command, sizeof(src[i].command), 1, f);
        fwrite(&src[i].data.dataInt, sizeof(src[i].data.dataInt), 1, f);
    }

    fclose(f);
}
