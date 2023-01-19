#include "game.hpp"
#include "globals.hpp"
#include "round_editor.hpp"
#include "hud.hpp"
#include "gui.hpp"

#include <filesystem>

Game::Game()
{
    // Temp
    maxWave = 20;

    mCurrentScene = Scene::MAIN_MENU;
    mCurrentLevel = 0;
    mAmountOfLevels = 0;

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
    mPlayField->ResetEntireClipdata();

    Cleanup();
}

void Game::Cleanup()
{
    std::vector<Tower*>* towers = mPlayer->GetTowers();

    for (std::vector<Tower*>::iterator _t = towers->begin(); _t != towers->end(); _t++)
        delete* _t;
    towers->clear();

    for (std::vector<Enemy*>::iterator _e = enemies.begin(); _e != enemies.end(); _e++)
        delete* _e;
    enemies.clear();

    for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); _p++)
        delete* _p;
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
        Scene_FreeplaySelection();
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

    enemies.insert(enemies.end(), enemiesQueue.begin(), enemiesQueue.end());
    enemiesQueue.clear();
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
    mCurrentScene = Gui::UpdateMainMenu();
}

void Game::Scene_LevelSelection()
{
    mCurrentScene = Gui::UpdateLevelSelection();
}

void Game::Scene_FreeplaySelection()
{
    mCurrentScene = Gui::UpdateFreeplaySelection();
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
    mCurrentScene = Gui::UpdateOptions();
}

uint8_t Game::CountLevels()
{
    if (mAmountOfLevels != 0)
        return mAmountOfLevels;

    std::filesystem::path path = std::filesystem::current_path().append("data/maps/");
    std::filesystem::directory_iterator dirIter = std::filesystem::directory_iterator(path);
    uint32_t fileCount = 0;

    for (auto& entry : dirIter)
    {
        if (entry.is_regular_file())
            fileCount++;
    }

    mAmountOfLevels = fileCount;
    return mAmountOfLevels;
}

void Game::StartLevel(uint8_t level)
{
    mCurrentLevel = level;

    RoundEditor::Load(roundInfo, (WAVES_PATH "Wave1"));

    Round::StartRound(roundInfo.data());

    std::string id = std::to_string(level);

    mPlayField->Load(std::string("Level").append(id).append(".bin"));
}
