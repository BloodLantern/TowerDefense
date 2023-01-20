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
    Player* GetPlayerSelf() const { return players[0]; }

    void InstantiatePlayer(std::string name, uint32_t uid);
    void RemovePlayer(uint32_t id);
    void AssignPlayerID(uint32_t uid) { mAssignedPlayerID = uid; }
    uint32_t GetAssignedPlayerID() { return mAssignedPlayerID; }

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

    uint16_t GetLife() { return mLife; }
    void DecreaseLife(uint16_t amount);
    void IncreaseLife(uint16_t amount);

    uint8_t CountLevels();
    void StartLevel(uint8_t level);

    bool IsFirstFrameOfRound() { return mIsFirstFrameOfRound; }

    std::vector<Player*> players;
private:
    uint16_t mLife = PLAYER_DEFAULT_HEALTH;

    PlayField* mPlayField = nullptr;

    uint32_t mAssignedPlayerID;

    bool mEnded = false;

    double_t mDeltaTime = 0.0;
    double_t mPlayingSpeedDeltaTime = 0.0;
    int32_t mPlayingSpeed = 1;

    Scene mCurrentScene;
    uint8_t mCurrentLevel;
    uint8_t mAmountOfLevels;

    bool mIsFirstFrameOfRound;

    std::string rpcDetails;
    std::string rpcLevelIndex;
    std::string rpcLargeImageKey;
    std::string rpcLargeImageText;

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

    void UpdateRichPresence();
};