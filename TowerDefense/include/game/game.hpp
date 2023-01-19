#pragma once

#include "playfield.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "round.hpp"
#include "scene.hpp"

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
    std::vector<Enemy*> enemiesQueue;
    std::vector<Projectile*> projectiles;
    std::vector<Projectile*> projectilesQueue;
    uint32_t currentWave = 1;
    uint32_t maxWave = 20;
    std::vector<RoundInfo> roundInfo;

    double_t GetPlayingSpeedDeltaTime() const { return mPlayingSpeedDeltaTime; }
    double_t GetDeltaTime() const { return mDeltaTime; }
    int32_t GetPlayingSpeed() const { return mPlayingSpeed; }
    void SetPlayingSpeed(int32_t speed) { mPlayingSpeed = speed; }
    void SetScene(Scene scene) { mCurrentScene = scene; }

    void Reset();
    void EndGame(bool lost);
    void Restart();

    uint8_t CountLevels();
    void StartLevel(uint8_t level);

    bool IsFirstFrameOfRound() { return mIsFirstFrameOfRound; }

private:
    PlayField* mPlayField = nullptr;
    Player* mPlayer = nullptr;

    bool mEnded = false;


    std::string mRpcDetailsText = "Details text";

    double_t mDeltaTime = 0.0;
    double_t mPlayingSpeedDeltaTime = 0.0;
    int32_t mPlayingSpeed = 1;

    Scene mCurrentScene;
    uint8_t mCurrentLevel;
    uint8_t mAmountOfLevels;

    bool mIsFirstFrameOfRound;

    void Scene_MainMenu();
    void Scene_LevelSelection();
    void Scene_FreeplaySelection();
    void Scene_InGame();
    void Scene_Options();
    void Scene_Bestiary();

    void DrawHud();
    void CheckEndRound();

    void UpdateTowers();
    void UpdateEnemies();
    void UpdateProjectiles();

    void Cleanup();
};