#pragma once

#include "playfield.hpp"
#include "player.hpp"
#include "enemy.hpp"

class Game {
public:
    Game();
    ~Game();

    void Draw();

    PlayField* GetPlayField() const { return mPlayField; }
    Player* GetPlayer() const { return mPlayer; }

    std::vector<Enemy*> mEnemies;
    uint32_t mCurrentWave;

    double_t GetPlayingSpeedDeltaTime() const { return mPlayingSpeedDeltaTime; }
    double_t GetDeltaTime() const { return mDeltaTime; }
    void SetPlayingSpeed(float speed) { mPlayingSpeed = speed; }

private:
    PlayField* mPlayField;
    Player* mPlayer;

    std::string mRpcDetailsText;

    double_t mDeltaTime;
    double_t mPlayingSpeedDeltaTime;
    float_t mPlayingSpeed;
};
