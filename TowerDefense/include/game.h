#pragma once

#include "PlayField.h"
#include "player.h"
#include "Enemy.h"

class Game {
public:
    Game();
    ~Game();

    void Draw();

    PlayField* GetPlayField() const { return mPlayField; }
    Player* GetPlayer() const { return mPlayer; }

    std::vector<Enemy*> mEnemies;
private:
    PlayField* mPlayField;
    Player* mPlayer;
};
