#pragma once

#include "PlayField.h"
#include "player.h"

class Game {
public:
    Game();
    ~Game();

    void Draw();

    PlayField* getPlayField() const { return m_playField; }
    Player* getPlayer() const { return m_player; }

private:
    PlayField* m_playField;
    Player* m_player;
};
