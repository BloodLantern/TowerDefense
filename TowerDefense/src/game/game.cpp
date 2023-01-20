#include "game.hpp"
#include "network/network_interface.hpp"
#include "globals.hpp"
#include "round_editor.hpp"
#include "hud.hpp"
#include "gui.hpp"
#include "bestiary.hpp"

#include <filesystem>

Game::Game()
{
    maxWave = 20;

    mCurrentScene = Scene::MAIN_MENU;
    mCurrentLevel = 0;
    mAmountOfLevels = 0;

    Hud::canInteract = true;
    mIsFirstFrameOfRound = false;

    mAssignedPlayerID = 0;
}

Game::~Game()
{
    delete mPlayField;

    for (size_t i = 0; i < players.size(); i++)
        delete players[i];
}

void Game::Init()
{
    mPlayField = new PlayField;
    // Create player instance representing self, aka the client
    players.push_back(new Player());
    mPlayingSpeed = 1;
    currentWave = 1;
    mDeltaTime = 0;
    mPlayingSpeedDeltaTime = 0;

    Bestiary::InitTextures();
}

void Game::Reset()
{
    mEnded = false;
    Hud::canInteract = true;
    currentWave = 1;
    SetPlayingSpeed(1);

    for (size_t i = 0; i < players.size(); i++)
        players[i]->Reset();
    
    mPlayField->ResetEntireClipdata();

    Cleanup();
}

void Game::EndGame(bool lost)
{
    mEnded = true;
    Hud::canInteract = false;
    SetPlayingSpeed(0);
}

void Game::Restart()
{
    Reset();
    RoundEditor::Load(roundInfo, (WAVES_PATH "Wave1"));
    Round::StartRound(roundInfo.data());
}

void Game::Cleanup()
{
    for (size_t i = 0; i < players.size(); i++)
    {
        std::vector<Tower*>* towers = players[i]->GetTowers();

        for (std::vector<Tower*>::iterator _t = towers->begin(); _t != towers->end(); _t++)
            delete* _t;
        towers->clear();
    }

    for (std::vector<Enemy*>::iterator _e = enemies.begin(); _e != enemies.end(); _e++)
        delete* _e;
    enemies.clear();

    for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); _p++)
        delete* _p;
    projectiles.clear();

    enemiesQueue.clear();
    projectilesQueue.clear();
}

void Game::UpdateRichPresence()
{
    DiscordRichPresence& rpc = Globals::gDiscordRpc;
    if (mCurrentScene == Scene::IN_GAME)
    {
        if (Globals::gNetwork.client->IsConnected() || Globals::gNetwork.IsServerStarted())
            rpc.state = "Playing Multiplayer";
        else
            rpc.state = "Playing Solo";
        rpcDetails = std::string("Wave ").append(std::to_string(currentWave));
        if (currentWave <= maxWave)
            rpcDetails = rpcDetails.append("/").append(std::to_string(maxWave));
        rpc.details = rpcDetails.c_str();

        rpcLevelIndex = std::to_string(mCurrentLevel);
        rpcLargeImageKey = "levelpreview" + rpcLevelIndex;
        rpcLargeImageText = "Level " + rpcLevelIndex;
        
        rpc.largeImageKey = rpcLargeImageKey.c_str();
        rpc.largeImageText = rpcLargeImageText.c_str();
        rpc.smallImageKey = "logo";
        rpc.smallImageText = "Fourmi Defense";
    }
    else
    {
        rpc.state = "Browsing menus";
        rpc.largeImageKey = "logo";
        rpc.largeImageText = "Fourmi Defense";
    }
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

        case Scene::BESTIARY:
            Scene_Bestiary();
    }

    UpdateRichPresence();
}

void Game::Shutdown()
{
    delete mPlayField;

    for (size_t i = 0; i < players.size(); i++)
        delete players[i];
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
        if (mLife == 0)
            Hud::DrawGameOver(position);
        else
            Hud::DrawWinScreen(position);
    }
}

void Game::CheckEndRound()
{
    if (!Round::HasEnded() || !enemies.empty())
        return;

    mIsFirstFrameOfRound = true;
    Round::GrantEndRoundMoney();
    
    if (currentWave == maxWave)
        EndGame(false);

    // Kill all projectiles
    for (std::vector<Projectile*>::iterator _p = projectiles.begin(); _p != projectiles.end(); )
    {
        Projectile* p = *_p;
        _p = projectiles.erase(_p);
        delete p;
    }

    // Reset attack cooldown for towers
    for (size_t i = 0; i < players.size(); i++)
    {
        std::vector<Tower*>* towers = players[i]->GetTowers();
        for (std::vector<Tower*>::iterator _t = towers->begin(); _t != towers->end(); _t++)
            (*_t)->SetTimeSinceLastAttack(DBL_MAX);
    }

    currentWave++;

    std::string fileName(WAVES_PATH "Wave");
    fileName.append(std::to_string(currentWave));

    RoundEditor::Load(roundInfo, fileName.c_str());
    Round::StartRound(roundInfo.data());
}

void Game::UpdateTowers()
{
    for (size_t i = 0; i < players.size(); i++)
    {
        std::vector<Tower*>* towers = players[i]->GetTowers();

        for (std::vector<Tower*>::iterator _t = towers->begin(); _t != towers->end(); )
        {
            Tower* t = *_t;

            t->OnUpdate();
            t->OnRender();

            if (t->toDelete)
            {
                // Destroy tower
                _t = towers->erase(_t);

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

    DrawHud();

    if (mLife <= 0)
    {
        EndGame(true);
        return;
    }

    mIsFirstFrameOfRound = false;

    if (mEnded)
        return;

    CheckEndRound();
}

void Game::Scene_Options()
{
    mCurrentScene = Gui::UpdateOptions();
}

void Game::Scene_Bestiary()
{
    mCurrentScene = Bestiary::Update();
}


void Game::DecreaseLife(uint16_t amount)
{
    // Prevent underflow
    if (amount > mLife)
        mLife = 0;
    else
        mLife -= amount;
}

void Game::IncreaseLife(uint16_t amount)
{
    uint32_t max = mLife + amount;

    // Prevent overflow
    if (max > USHRT_MAX)
        mLife = USHRT_MAX;
    else
        mLife = max;
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
    mCurrentScene = Scene::IN_GAME;

    Restart();
    mCurrentLevel = level;

    RoundEditor::Load(roundInfo, (WAVES_PATH "Wave1"));

    Round::StartRound(roundInfo.data());

    std::string id = std::to_string(level);

    mPlayField->Load(std::string("Level").append(id).append(".bin"));
    mIsFirstFrameOfRound = true;

    if (Globals::gNetwork.IsServerStarted())
        Globals::gNetwork.client->NotifyStartOfGame(level);
}

void Game::InstantiatePlayer2(std::string name, uint32_t uid)
{
    if (Globals::gNetwork.IsServerStarted())
    {
        // Don't instantiate yourself
        if (uid - 1 == 0)
            return;

        // Server is running, we are player 0
        players.push_back(new Player(name, uid));
        // Self;Player 1

        mAssignedPlayerID = 0;
    }
    else
    {
        // Server isn't running, we are player 1
        Player* self = players[0];
        players.clear();
        players.push_back(new Player(name, uid));
        players.push_back(self);
        // Player 0;Self
        
        mAssignedPlayerID = 1;
    }
}

void Game::RemovePlayer(uint32_t id)
{

}
