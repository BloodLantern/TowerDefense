#include "hud.hpp"

#include "game.hpp"
#include "globals.hpp"

bool Hud::canInteract;

void Hud::DrawHealth(ImVec2 position)
{
    Texture* tex = Globals::gResources->GetTexture("ui\\heart_icon");

    ImVec2 posMin(position);
    ImVec2 posMax(posMin.x + 40, posMin.y + 40);
    Globals::gDrawList->AddImage(tex->id, posMin, posMax);

    ImVec2 pos(posMax.x + 10, posMin.y + 5);

    char buff[20];
    sprintf_s(buff, "%d", Globals::gGame->GetPlayer()->GetLife());
    ImGui::PushFont(Globals::gFontSemiBig);
    Globals::gDrawList->AddText(pos, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), buff);
    ImGui::PopFont();
}

void Hud::DrawMoney(ImVec2 position)
{
    Texture* tex = Globals::gResources->GetTexture("ui\\money_icon");

    ImVec2 posMin(position);
    ImVec2 posMax(posMin.x + 40, posMin.y + 40);
    Globals::gDrawList->AddImage(tex->id, posMin, posMax, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(0xD3, 0xC5, 0x3B, 0xFF));

    ImVec2 pos(posMax.x + 10, posMin.y + 5);

    char buff[20];
    sprintf_s(buff, "%d", Globals::gGame->GetPlayer()->GetMoney());
    ImGui::PushFont(Globals::gFontSemiBig);
    Globals::gDrawList->AddText(pos, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), buff);
    ImGui::PopFont();
}

void Hud::DrawRounds(ImVec2 position)
{
    char buff[35];
    sprintf_s(buff, "Wave %d / %d", Globals::gGame->currentWave, Globals::gGame->maxWave);
    ImGui::PushFont(Globals::gFontSemiBig);
    Globals::gDrawList->AddText(position, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), buff);
    ImGui::PopFont();
}

void Hud::DrawSpeed(ImVec2 position)
{
    ImGui::PushFont(Globals::gFontBig);

    ImVec2 mouse = Globals::gIO->MousePos;
    //mouse.x -= Globals::gGridX;
    //mouse.y -= Globals::gGridY;

    Globals::gDrawList->AddText(position, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), "x1");
    if (canInteract && mouse.x > position.x && mouse.y > position.y && mouse.x < position.x + 40 && mouse.y < position.y + 40
        && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Globals::gGame->SetPlayingSpeed(1);
    }

    position.x += 50;
    Globals::gDrawList->AddText(position, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), "x2");
    if (canInteract && mouse.x > position.x && mouse.y > position.y && mouse.x < position.x + 40 && mouse.y < position.y + 40
        && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Globals::gGame->SetPlayingSpeed(2);
    }

    position.x += 50;
    Globals::gDrawList->AddText(position, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), "x4");
    if (canInteract && mouse.x > position.x && mouse.y > position.y && mouse.x < position.x + 40 && mouse.y < position.y + 40
        && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Globals::gGame->SetPlayingSpeed(4);
    }

    position.x += 50;
    Globals::gDrawList->AddText(position, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), "x0");
    if (canInteract && mouse.x > position.x && mouse.y > position.y && mouse.x < position.x + 40 && mouse.y < position.y + 40
        && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Globals::gGame->SetPlayingSpeed(0);
    }

    ImGui::PopFont();
}

void Hud::DrawGameOver(ImVec2 position)
{
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(ImVec2(500, 250));
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0x64 / 255.f, 0x6c / 255.f, 0x73 / 255.f, 0xFF / 255.f));

    if (ImGui::Begin("Game over", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove))
    {
        ImGui::PushFont(Globals::gFontBig);
        ImGui::SetCursorPosX(250.f - ImGui::CalcTextSize("GAME OVER").x / 2);
        ImGui::Text("GAME OVER");
        ImGui::PopFont();

        ImGui::SetCursorPos(ImVec2(250.f - 120.f / 2, 250.f / 2 - 40.f / 2));

        ImGui::PushFont(Globals::gFontMedium);
        if (ImGui::Button("Retry", ImVec2(120, 40)))
            Globals::gGame->Restart();
        ImGui::PopFont();
    }

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
