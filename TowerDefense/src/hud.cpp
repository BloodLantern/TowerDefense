#include "hud.hpp"

#include "game.hpp"
#include "globals.hpp"

void Hud::DrawHealth(ImVec2 position)
{
    Texture* tex = Globals::gResources->GetTexture("ui\\heart");

    ImVec2 posMin(position);
    ImVec2 posMax(posMin.x + 40, posMin.y + 40);
    Globals::gDrawList->AddImage(tex->id, posMin, posMax, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(0xD3, 0xC5, 0x3B, 0xFF));

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

}

