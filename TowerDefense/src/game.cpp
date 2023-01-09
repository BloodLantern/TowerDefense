#include "game.h"

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

    for (std::vector<Tower*>::iterator t = mPlayer->GetTowers()->begin(); t != mPlayer->GetTowers()->end(); t++)
        (*t)->Draw();
}
