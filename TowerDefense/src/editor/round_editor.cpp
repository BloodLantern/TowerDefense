#include "round_editor.hpp"
#include "gui.hpp"
#include "imgui.h"
#include "globals.hpp"

#include <iostream>

static const char* const sCommandNames[] = {
    "Enemy",
    "Cooldown",
    "GFX effect",
    "SFX",
    "Loop start",
    "Loop end",
    "End"
};

static const char* const sEnemyNames[] = {
    "Ant",
    "Mite",
    "Gold scarab",
    "Ladybug",
    "Red ant",
    "Ant colony (Boss)",
    "Trilobite",
    "Firefly",
};

std::vector<RoundInfo> RoundEditor::mRoundInfo;
std::vector<bool> RoundEditor::mSeparators;
char RoundEditor::mFileName[30];

void RoundEditor::Update()
{
	if (ImGui::Begin("Wave editor", &Gui::openedWindows[GUI_WINDOW_ID_WAVE_EDITOR]))
	{
        if (ImGui::Button("Add"))
        {
            mRoundInfo.push_back(RoundInfo(ROUND_COMMAND_SPAWN_ENEMY, 0u));
            mSeparators.push_back(false);
        }

        ImGui::SameLine();
        RoundEditor::HandleClear();
        ImGui::SameLine();
        RoundEditor::HandleCurrentWaveLoad();
        ImGui::SameLine();
        RoundEditor::HandleTest();
        ImGui::SameLine();
        RoundEditor::HandleSaveLoad();
        RoundEditor::DisplayTable();
	}

	ImGui::End();
}

void RoundEditor::DisplayTable()
{
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + 220, cursor.y));
    if (!ImGui::BeginTable("Command table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        return;

    ImGui::TableSetupColumn("Command");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();

    for (size_t row = 0; row < mRoundInfo.size(); row++)
    {
        RoundInfo* round = &mRoundInfo[row];

        if (mSeparators[row])
            ImGui::Dummy(ImVec2(0, 10));

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
    size_t roundID = -1;
    for (std::vector<RoundInfo>::iterator _r = mRoundInfo.begin(); _r != mRoundInfo.end(); )
    {
        roundID++;
        RoundInfo* round = &*_r;
        ImGui::PushID(round);
        
        if (mSeparators[roundID])
            ImGui::Dummy(ImVec2(0, 10));

        if (ImGui::Button("Delete"))
        {
            _r = mRoundInfo.erase(_r);
            mSeparators.erase(mSeparators.begin() + roundID);
            roundID--;
            ImGui::PopID();
            continue;
        }

        ImGui::SameLine();
        if (ImGui::Button("Insert"))
        {
            _r = mRoundInfo.insert(_r, RoundInfo(ROUND_COMMAND_SPAWN_ENEMY, 0u));
            mSeparators.insert(mSeparators.begin() + roundID, false);
            roundID--;
            ImGui::PopID();
            continue;
        }

        if (_r != mRoundInfo.begin())
        {
            ImGui::SameLine();
            if (ImGui::Button("Separator"))
                mSeparators[roundID] = (mSeparators[roundID] ^ true);
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
        case ROUND_COMMAND_PLAY_SOUND:
            ImGui::InputScalar("##input", ImGuiDataType_U32, &round->data.dataInt);
            break;

        case ROUND_COMMAND_LOOP_START:
        {
            uint32_t step = 1;
            ImGui::InputScalar("##input", ImGuiDataType_U32, &round->data.dataInt, &step);
            break;
        }

        case ROUND_COMMAND_LOOP_END:
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
            mSeparators.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("No", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void RoundEditor::HandleCurrentWaveLoad()
{
    if (!ImGui::Button("Load current"))
        return;

    std::string fullPath(WAVES_PATH);
    std::string fileName("Wave");
    fileName.append(std::to_string(Globals::gGame->currentWave));
    fullPath.append(fileName);

    RoundEditor::Load(mRoundInfo, fullPath.c_str());
    mSeparators.clear();
    mSeparators.resize(mRoundInfo.size());

    size_t i = 0;
    for (; i < fileName.length(); i++)
        mFileName[i] = fileName.c_str()[i];

    mFileName[i] = '\0';
}

void RoundEditor::HandleTest()
{
    if (!ImGui::Button("Test"))
        return;

    Globals::gGame->Restart();
    Round::StartRound(mRoundInfo.data());
}


void RoundEditor::HandleSaveLoad()
{
    ImGui::Dummy(ImVec2(80, 0));
    ImGui::SameLine();

    if (ImGui::Button("Save"))
        Save(mRoundInfo, std::string(WAVES_PATH).append(mFileName).c_str());

    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
        Load(mRoundInfo, std::string(WAVES_PATH).append(mFileName).c_str());
        mSeparators.clear();
        mSeparators.resize(mRoundInfo.size());
    }

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
