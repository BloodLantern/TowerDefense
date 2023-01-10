#include "game.h"

Texture mTempTex;

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

    if (ImGui::IsKeyPressed(ImGuiKey_Space, false))
    {
        int32_t screenX;
        int32_t screenY;
        mPlayField->GetPixelPositionFromGrid(0, 7, screenX, screenY);

        Enemy* e = new Enemy(Point2(screenX, screenY), 0, 0, 0);
        mTempTex = ImGuiUtils::LoadTexture("assets/ant.png");
        e->SetTexture(&mTempTex);
        e->SetScale(.1f);

        mEnemies.push_back(e);
    }

    
    for (std::vector<Tower*>::iterator _t = mPlayer->GetTowers()->begin(); _t != mPlayer->GetTowers()->end(); _t++)
    {
        Tower* t = *_t;
        t->OnRender();
    }

    for (std::vector<Enemy*>::iterator _e = mEnemies.begin(); _e != mEnemies.end(); _e++)
    {
        Enemy* e = *_e;
        e->StickToPath();
        e->OnRender();
    }
}
