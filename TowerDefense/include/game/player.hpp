#pragma once

#include "tower.hpp"

#include <stdint.h>
#include <vector>

#define PLAYER_DEFAULT_MONEY 200
#define PLAYER_DEFAULT_HEALTH 10

class Player {
public:
    Player();
    Player(std::string username);
    ~Player();

    std::vector<Tower*>* GetTowers() { return &mTowers; }

    uint32_t GetMoney() const { return mMoney; }
    uint32_t& GetMoney() { return mMoney; }
    void DecreaseMoney(uint32_t amount);
    void IncreaseMoney(uint32_t amount);

    void Reset();

private:
    static uint32_t playerGlobalID;

    uint32_t mMoney = PLAYER_DEFAULT_MONEY;

    std::vector<Tower*> mTowers;

    // Network related
    std::string mUsername;
    uint32_t mPlayerID;
};
