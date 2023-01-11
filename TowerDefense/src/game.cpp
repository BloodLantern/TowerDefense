#include "game.h"
#include "Round.h"

Game::Game()
    : mPlayField(new PlayField), mPlayer(new Player)
{

}

Game::~Game()
{
    delete mPlayField;
    delete mPlayer;
}

void Game::Draw()
{
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
}
