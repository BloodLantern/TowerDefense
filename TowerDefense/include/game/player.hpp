#pragma once

#include "tower.hpp"

#include <stdint.h>
#include <vector>

#define PLAYER_DEFAULT_MONEY 20000
#define PLAYER_DEFAULT_HEALTH 1000

class Player {
public:
    Player();
    ~Player();

    std::vector<Tower*>* GetTowers() { return &mTowers; }

    void OnUpdate();

    uint16_t GetLife() const { return mLife; }
    uint16_t& GetLife() { return mLife; }
    void DecreaseLife(uint16_t amount);
    void IncreaseLife(uint16_t amount);

    uint32_t GetMoney() const { return mMoney; }
    uint32_t& GetMoney() { return mMoney; }
    void DecreaseMoney(uint32_t amount);
    void IncreaseMoney(uint32_t amount);

    void Reset();

private:
    uint32_t mMoney = PLAYER_DEFAULT_MONEY;
    uint16_t mLife = PLAYER_DEFAULT_HEALTH;

    std::vector<Tower*> mTowers;
};
