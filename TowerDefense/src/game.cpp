#include "game.h"

Game::Game()
    : m_playField(new PlayField), m_player(new Player)
{

}

Game::~Game()
{
    delete m_playField;
    delete m_player;
}

void Game::Draw()
{
    m_playField->draw();
}
