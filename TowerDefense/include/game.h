#pragma once

#include "PlayField.h"
#include "player.h"

class Game {
public:
    Game();
    ~Game();

    void Draw();

    PlayField* GetPlayField() const { return mPlayField; }
    Player* GetPlayer() const { return mPlayer; }

private:
    PlayField* mPlayField;
    Player* mPlayer;
};