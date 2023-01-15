#pragma once

#include "playfield.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "round.hpp"

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Update();
    void Shutdown();

    PlayField* GetPlayField() const { return mPlayField; }
    Player* GetPlayer() const { return mPlayer; }

    std::vector<Enemy*> enemies;
    std::vector<Projectile*> projectiles;
    uint32_t currentWave;
    uint32_t maxWave;
    std::vector<RoundInfo> roundInfo;

    double_t GetPlayingSpeedDeltaTime() const { return mPlayingSpeedDeltaTime; }
    double_t GetDeltaTime() const { return mDeltaTime; }
    int32_t GetPlayingSpeed() const { return mPlayingSpeed; }
    void SetPlayingSpeed(int32_t speed) { mPlayingSpeed = speed; }

    void EndGame(bool lost);
    void Restart();

private:
    PlayField* mPlayField;
    Player* mPlayer;

    bool mEnded;

    std::string mRpcDetailsText;

    double_t mDeltaTime;
    double_t mPlayingSpeedDeltaTime;
    int32_t mPlayingSpeed;

    void DrawHud();
    void CheckEndRound();

    void UpdateTowers();
    void UpdateEnemies();
    void UpdateProjectiles();

    void Cleanup();
};
