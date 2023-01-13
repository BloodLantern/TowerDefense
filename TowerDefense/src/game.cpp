#include "game.hpp"
#include "globals.hpp"
#include "rounds_data.hpp"
#include "round.hpp"

Game::Game()
    : mPlayField(new PlayField), mPlayer(new Player), mPlayingSpeed(1), mCurrentWave(1)
{
    mDeltaTime = 0;
    mPlayingSpeedDeltaTime = 0;
}

Game::~Game()
{
    delete mPlayField;
    delete mPlayer;
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

    for (std::vector<Enemy*>::iterator _e = mEnemies.begin(); _e != mEnemies.end(); )
    {
        Enemy* e = *_e;

        e->OnUpdate();
        if (e->mToDelete)
        {
            // Kill enemy
            _e = mEnemies.erase(_e);
            delete e;
            continue;
        }

        e->OnRender();
        _e++;
    }

    if (Round::HasEnded() && mEnemies.empty())
    {
        Round::GrantEndRoundMoney();
        mCurrentWave++;
        mRpcDetailsText = std::string("Wave ").append(std::to_string(mCurrentWave)).append("/20");
        Globals::gDiscordRpc.details = mRpcDetailsText.c_str();

        Round::StartRound(sLevel1_Wave1);
    }
}
