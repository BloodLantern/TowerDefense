#include "game.hpp"
#include "globals.hpp"
#include "round_editor.hpp"

Game::Game()
{
    RoundEditor::Load(roundInfo, "Wave1");

    Round::StartRound(roundInfo.data());
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

void Game::Draw()
{
    mDeltaTime = Globals::gIO->DeltaTime;
    mPlayingSpeedDeltaTime = mDeltaTime * mPlayingSpeed;
    
    ImGui::Begin("Delta time");
    ImGui::SliderFloat("Play speed", &mPlayingSpeed, .1f, 5.f);
    ImGui::End();

    mPlayField->Draw();

    Round::OnUpdate();

    mPlayer->OnRender();
    
    for (std::vector<Tower*>::iterator _t = mPlayer->GetTowers()->begin(); _t != mPlayer->GetTowers()->end(); _t++)
    {
        Tower* t = *_t;

        t->OnUpdate();
        t->OnRender();
    }

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

    if (Round::HasEnded() && enemies.empty())
    {
        Round::GrantEndRoundMoney();
        currentWave++;
        mRpcDetailsText = std::string("Wave ").append(std::to_string(currentWave)).append("/20");
        Globals::gDiscordRpc.details = mRpcDetailsText.c_str();

        Round::StartRound(roundInfo.data());
    }
}

void Game::Shutdown()
{
    delete mPlayField;
    delete mPlayer;
}
