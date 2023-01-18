#include "game.hpp"
#include "globals.hpp"
#include "round_editor.hpp"
#include "hud.hpp"

Game::Game()
{
    RoundEditor::Load(roundInfo, "data/waves/Wave1");

    Round::StartRound(roundInfo.data());
    
    // Temp
    maxWave = 20;

    mCurrentScene = Scene::MAIN_MENU;
    mCurrentLevel = 0;

    Hud::canInteract = true;
}

Game::~Game()
{
    delete mPlayField;
    delete mPlayer;
}

void Game::Init()
{
    mPlayField = new PlayField;
    mPlayer = new Player;
    mPlayingSpeed = 1;
    currentWave = 1;
    mDeltaTime = 0;
    mPlayingSpeedDeltaTime = 0;
}

void Game::EndGame(bool lost)
{
    mEnded = true;
    Hud::canInteract = false;
    SetPlayingSpeed(0);
}

void Game::Restart()
{
    mEnded = false;
    Hud::canInteract = true;

    currentWave = 1;
    RoundEditor::Load(roundInfo, (WAVES_PATH "Wave1"));
    Round::StartRound(roundInfo.data());
    SetPlayingSpeed(1);
    mPlayer->Reset();
    
    Cleanup();
}

void Game::Cleanup()
{
    std::vector<Tower*>* towers = mPlayer->GetTowers();

    for (std::vector<Tower*>::iterator _t = towers->begin(); _t != towers->end(); _t++)
        delete *_t;
    towers->clear();

    for (std::vector<Enemy*>::iterator _e = enemies.begin(); _e != enemies.end(); _e++)
        delete *_e;
    enemies.clear();

    for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); _p++)
        delete *_p;
    projectiles.clear();
}

void Game::Update()
{
    mDeltaTime = Globals::gIO->DeltaTime;
    mPlayingSpeedDeltaTime = mDeltaTime * mPlayingSpeed;

    switch (mCurrentScene)
    {
        case Scene::MAIN_MENU:
            Scene_MainMenu();
            break;

        case Scene::LEVEL_SELECTION:
            Scene_LevelSelection();
            break;

        case Scene::FREEPLAY_SELECTION:
            Scene_FreeSelection();
            break;

        case Scene::IN_GAME:
            Scene_InGame();
            break;

        case Scene::OPTIONS:
            Scene_Options();
            break;
    }
}

void Game::Shutdown()
{
    delete mPlayField;
    delete mPlayer;
}

void Game::DrawHud()
{
    ImVec2 position(Globals::gGridX + 10, Globals::gGridY + 10);
    
    Hud::DrawHealth(position);

    position.x += 100;
    Hud::DrawMoney(position);

    position.x += 300;
    Hud::DrawRounds(position);

    position = ImVec2(Globals::gGridX + Globals::gWindowWidth - 230, Globals::gGridY + Globals::gWindowHeight - 70);
    Hud::DrawSpeed(position);

    if (mEnded)
    {
        position = ImVec2(Globals::gGridX + Globals::gWindowWidth / 2 - 250, Globals::gGridY + Globals::gWindowHeight / 2 - 125);
        Hud::DrawGameOver(position);
    }
}

void Game::CheckEndRound()
{
    if (!Round::HasEnded() || !enemies.empty())
        return;
     
    Round::GrantEndRoundMoney();

    // Kill all projectiles
    for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); )
    {
        Projectile* p = *_p;
        _p = projectiles.erase(_p);
        delete p;
    }

    // Reset attack cooldown for towers
    for (std::vector<Tower*>::iterator _t = mPlayer->GetTowers()->begin(); _t != mPlayer->GetTowers()->end(); _t++)
        (*_t)->SetTimeSinceLastAttack(DBL_MAX);

    currentWave++;
    mRpcDetailsText = std::string("Wave ").append(std::to_string(currentWave)).append("/").append(std::to_string(maxWave));
    Globals::gDiscordRpc.details = mRpcDetailsText.c_str();

    std::string fileName(WAVES_PATH "Wave");
    fileName.append(std::to_string(currentWave));

    RoundEditor::Load(roundInfo, fileName.c_str());
    Round::StartRound(roundInfo.data());
}

void Game::UpdateTowers()
{
    for (std::vector<Tower*>::iterator _t = mPlayer->GetTowers()->begin(); _t != mPlayer->GetTowers()->end(); )
    {
        Tower* t = *_t;

        t->OnUpdate();
        t->OnRender();

        if (t->toDelete)
        {
            // Destroy tower
            _t = mPlayer->GetTowers()->erase(_t);

            // Set its projectiles' pointers to nullptr
            for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); _p++)
            {
                Projectile* p = *_p;
                if (p->GetOwner() == t)
                    p->SetOwner(nullptr);

            }
            delete t;
            continue;
        }

        _t++;
    }
}
void Game::UpdateEnemies()
{

    for (std::vector<Enemy*>::iterator _e = enemies.begin(); _e != enemies.end(); )
    {
        Enemy* e = *_e;

        e->OnUpdate();
        if (e->toDelete)
        {
            // Kill enemy
            _e = enemies.erase(_e);
            delete e;
            continue;
        }

        e->OnRender();
        _e++;
    }
}

void Game::UpdateProjectiles()
{
    for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); )
    {
        Projectile* p = *_p;

        p->OnUpdate();

        if (p->toDelete)
        {
            // Kill projectile
            _p = projectiles.erase(_p);
            delete p;
            continue;
        }

        p->OnRender();
        _p++;
    }

    projectiles.insert(projectiles.end(), projectilesQueue.begin(), projectilesQueue.end());
    projectilesQueue.clear();
}



void Game::Scene_MainMenu()
{
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
            mCurrentScene = Scene::LEVEL_SELECTION;

        cursor.y += 100;
        ImGui::SetCursorPos(cursor);
        if (ImGui::Button("Freeplay", buttonSize))
            mCurrentScene = Scene::FREEPLAY_SELECTION;

        cursor.y += 100;
        ImGui::SetCursorPos(cursor);
        if (ImGui::Button("Options", buttonSize))
            mCurrentScene = Scene::OPTIONS;

        /*cursor.y += 100;
        ImGui::SetCursorPos(cursor);
        if (ImGui::Button("Bestiary", buttonSize))
            mCurrentScene = Scene::BESTIARY;*/

        ImGui::PopFont();
    }

    ImGui::End();
}

void Game::Scene_LevelSelection()
{

}

void Game::Scene_FreeSelection()
{

}

void Game::Scene_InGame()
{
    mPlayField->Draw();

    Round::OnUpdate();

    UpdateTowers();
    UpdateEnemies();
    UpdateProjectiles();

    mPlayer->OnUpdate();

    DrawHud();

    if (mEnded)
        return;

    CheckEndRound();
}

void Game::Scene_Options()
{

}

